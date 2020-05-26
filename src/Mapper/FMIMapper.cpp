#include "mapper/FMIMapper.h"

void FMIMapper::setConfiguration() {

}

int FMIMapper::readConfiguration(configVariants_t configVariants) {
	std::cout << "Read Configuration of FMI Mapper" << std::endl;

	if (std::get_if<FMIInterfaceConfig>(&configVariants) == nullptr) {
		std::cout << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	FMIInterfaceConfig interfaceConfig = std::get<FMIInterfaceConfig>(configVariants);

	//TODO retrieve FMU location from SSP -  currently interprets ssp file node as FMU path for testing

	std::unique_ptr<fmi4cpp::fmi2::fmu> fmu(new fmi4cpp::fmi2::fmu(interfaceConfig.models));
	if (!fmu->supports_cs()) {
		// FMU contains no cs model
		return 216373;
	}

	// load co-simulation description from FMU
	auto coSimFMU = fmu->as_cs_fmu();
	auto modelDescription = coSimFMU->get_model_description();

	//cast iSimulationData to FMIBridge to move co-simulation FMU into it
	auto bridge = std::dynamic_pointer_cast<FMIBridge>(owner.lock());
	bridge->coSimFMU = std::move(coSimFMU);

	//TODO need basename definitions for base interface
	for (const auto& var : *modelDescription->model_variables) {
		// cannot switch on outputVar.value_type because fmi4cpp uses strings and cannot use getType because type name 'Real' is not defined (maps to some floating point type, typically double) 
		if (var.is_boolean()) {
			if (fmi4cpp::fmi2::causality::input == var.causality) {
				config.boolInputList.push_back(NamesAndIndex(var.name, var.name, (int)config.boolInputList.size()));
			}
			else if (fmi4cpp::fmi2::causality::output == var.causality) {
				config.boolOutputList.push_back(NamesAndIndex(var.name, var.name, (int)config.boolOutputList.size()));
			}
		}
		else if (var.is_enumeration() || var.is_integer()) {
			if (fmi4cpp::fmi2::causality::input == var.causality) {
				config.intInputList.push_back(NamesAndIndex(var.name, var.name, (int)config.intInputList.size()));
			}
			else if (fmi4cpp::fmi2::causality::output == var.causality) {
				config.intOutputList.push_back(NamesAndIndex(var.name, var.name, (int)config.intOutputList.size()));
			}
		}
		else if (var.is_real()) {
			if (typeid(fmi2Real) == typeid(double)) {
				if (fmi4cpp::fmi2::causality::input == var.causality) {
					config.doubleInputList.push_back(NamesAndIndex(var.name, var.name, (int)config.doubleInputList.size()));
				}
				else if (fmi4cpp::fmi2::causality::output == var.causality) {
					config.doubleOutputList.push_back(NamesAndIndex(var.name, var.name, (int)config.doubleOutputList.size()));
				}
			}
			else {
				if (fmi4cpp::fmi2::causality::input == var.causality) {
					config.floatInputList.push_back(NamesAndIndex(var.name, var.name, (int)config.floatInputList.size()));
				}
				else if (fmi4cpp::fmi2::causality::output == var.causality) {
					config.floatOutputList.push_back(NamesAndIndex(var.name, var.name, (int)config.floatOutputList.size()));
				}
			}
		}
		else /*if (var.is_string())*/ {
			if (fmi4cpp::fmi2::causality::input == var.causality) {
				config.stringInputList.push_back(NamesAndIndex(var.name, var.name, (int)config.stringInputList.size()));
			}
			else if (fmi4cpp::fmi2::causality::output == var.causality) {
				config.stringOutputList.push_back(NamesAndIndex(var.name, var.name, (int)config.stringOutputList.size()));
			}
		}
	}

	return 0;
}