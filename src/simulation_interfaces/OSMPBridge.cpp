#include "simulation_interfaces/OSMPBridge.h"

int OSMPBridge::readConfiguration(configVariants_t configVariants) {
	if (std::get_if<OSMPInterfaceConfig>(&configVariants) == nullptr) {
		std::cout << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}
	OSMPInterfaceConfig interfaceConfig = std::get<OSMPInterfaceConfig>(configVariants);

	std::unique_ptr<fmi4cpp::fmi2::fmu> fmu = std::make_unique<fmi4cpp::fmi2::fmu>(interfaceConfig.model);
	if (!fmu->supports_cs()) {
		// FMU contains no cs model
		std::cerr << __FUNCTION__ << "FMU contains no cs (CoSimulation) model" << std::endl;
		return 1;
	}

	// load co-simulation description from FMU
	coSimFMU = fmu->as_cs_fmu();
	auto modelDescription = coSimFMU->get_model_description();
	return 0;
}


int OSMPBridge::init(float starttime) {
	//Instance name cannot be set with FMU4cpp. The model identifier is used automatically instead
	coSimSlave = coSimFMU->new_instance();

	coSimSlave->setup_experiment((fmi2Real)starttime);
	coSimSlave->enter_initialization_mode();
	coSimSlave->exit_initialization_mode();
	return 0;
}

int OSMPBridge::doStep(double stepSize) {
	//TODO
	//which parts of FMIBridge::doStep are needed?
		//TODO set independent tunable parameters
	//TODO set continuous- and discrete-time inputs and optionally also the derivatives of the former

	//TODO support rollback in case step is incomplete?
	auto preStepState = OSMPFMUSlaveStateWrapper::tryGetStateOf(coSimSlave);

	//TODO step by stepSize
	if (!coSimSlave->step(stepSize)) {
		while (fmi4cpp::status::Pending == coSimSlave->last_status()) {
			//wait for asynchronous fmu to finish
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		switch (coSimSlave->last_status()) {
		case fmi4cpp::status::Fatal:
			return -1;//TODO decide on common error return values
		case fmi4cpp::status::Error:
			return -2;//TODO decide on common error return values
		case fmi4cpp::status::Discard:
			//If a pre step state could be captured and the slave supports step size variation, try performing smaller substeps instead of one stepSize step
			if (!preStepState || !coSimSlave->get_model_description()->can_handle_variable_communication_step_size) {
				return 2;
			}
			//restore state before failed step
			coSimSlave->set_fmu_state(preStepState.value().state);
			// perform some smaller substeps
			int substeps = 2;
			for (int i = 0; i < substeps; i++) {
				int err = doStep(stepSize / 2);
				if (0 != err) {
					return 2;
				}
			}
		}
	}
	return 0;
}



int OSMPBridge::disconnect() {
	return OSIBridge::disconnect();
}

int OSMPBridge::writeToInternalState() {
	auto const model_description = coSimFMU->get_model_description();
	//iterate over unknowns declared as output
	for (auto const& unknown : model_description->model_structure->outputs) {
		// use index to translate unknown into scalar_variable. FMU ScalarVariable index begins at 1
		auto const& outputVar = (*model_description->model_variables.get())[unknown.index - 1];
		if (outputVar.is_integer()) {
			fmi2Integer integer;
			coSimSlave->read_integer(outputVar.value_reference, integer);
			saveToAddressMap(inputAddresses, outputVar.name, integer);
		}
	}
	if (!valid) {
		std::cout << "OSMP config not valid" << std::endl;
		return 1;
	}
	//write all messages to internal state
	for (auto& address : inputAddresses) {
		OSIBridge::writeToInternalState(address.second);
	}
	return 0;
}

int OSMPBridge::readFromInternalState() {
	auto const model_description = coSimFMU->get_model_description();
	//iterate over unknowns declared as output and create AddressMap
	for (auto const& inputVar : *(model_description->model_variables)) {
		if (inputVar.causality == fmi4cpp::fmi2::causality::input && inputVar.is_integer()) {
			fmi2Integer integer;
			coSimSlave->read_integer(inputVar.value_reference, integer);
			saveToAddressMap(outputAddresses, inputVar.name, integer);
		}
	}
	//write message
	for (auto& address : outputAddresses) {
		OSIBridge::readFromInternalState(address.second);
	}
	//set pointers of messages in fmi
	for (auto const& inputVar : *(model_description->model_variables)) {
		if (inputVar.causality == fmi4cpp::fmi2::causality::input && inputVar.is_integer()) {
			for (auto address : outputAddresses) {
				if (inputVar.name.find(address.first) != std::string::npos) {
					if (inputVar.name.find(".hi") != std::string::npos) {
						coSimSlave->write_integer(inputVar.value_reference, address.second.addr.base.hi);
					}
					else if (inputVar.name.find(".lo") != std::string::npos) {
						coSimSlave->write_integer(inputVar.value_reference, address.second.addr.base.lo);
					}
					else if (inputVar.name.find(".size") != std::string::npos) {
						coSimSlave->write_integer(inputVar.value_reference, address.second.size);
					}
				}
			}
		}
	}

	return 0;
}

void OSMPBridge::saveToAddressMap(std::map<std::string, address> &addressMap, std::string name, int value) {
	//check for normal fmi variables count and valid
	if (name == "count") {
		this->count = value;
		return;
	}
	if (name == "valid") {
		this->valid = value;
		return;
	}

	if (0 == name.compare(name.length() - 8, 8, ".base.hi")) {
		std::string prefixWithIndex = name.substr(0, name.length() - 8);

		if (addressMap.find(prefixWithIndex) == addressMap.end()) {
			address a;
			a.addr.base.hi = value;
			a.name = prefixWithIndex;
			addressMap.insert({ prefixWithIndex , a});
		}
		else {
			addressMap.at(prefixWithIndex).addr.base.hi = value;
		}
	}
	else if (0 == name.compare(name.length() - 8, 8, ".base.lo")) {
		std::string prefixWithIndex = name.substr(0, name.length() - 8);

		if (addressMap.find(prefixWithIndex) == addressMap.end()) {
			address a;
			a.addr.base.lo = value;
			a.name = prefixWithIndex;
			addressMap.insert({ prefixWithIndex , a });
		}
		else {
			addressMap.at(prefixWithIndex).addr.base.lo = value;
		}
	}
	else if (0 == name.compare(name.length() - 5, 5, ".size")) {
		std::string prefixWithIndex = name.substr(0, name.length() - 5);

		if (addressMap.find(prefixWithIndex) == addressMap.end()) {
			address a;
			a.size = value;
			a.name = prefixWithIndex;
			addressMap.insert({ prefixWithIndex , a });
		}
		else {
			addressMap.at(prefixWithIndex).size = value;
		}
	}
}

inline OSMPBridge::OSMPFMUSlaveStateWrapper::OSMPFMUSlaveStateWrapper(std::shared_ptr<fmi4cpp::fmi2::cs_slave> slave) {
	slave->get_fmu_state(state);
	coSimSlave = slave;
}

inline OSMPBridge::OSMPFMUSlaveStateWrapper::~OSMPFMUSlaveStateWrapper() {
	coSimSlave->free_fmu_state(state);
}

std::optional<OSMPBridge::OSMPFMUSlaveStateWrapper> OSMPBridge::OSMPFMUSlaveStateWrapper::tryGetStateOf(std::shared_ptr<fmi4cpp::fmi2::cs_slave> slave) {
	if (slave->get_model_description()->can_get_and_set_fmu_state) {
		return OSMPBridge::OSMPFMUSlaveStateWrapper(slave);
	}
	return std::nullopt;
}
