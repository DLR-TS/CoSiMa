#include "mapper/FMIMapper.h"

void FMIMapper::setConfiguration() {

}

int FMIMapper::readConfiguration(configVariants_t configVariants) {
	std::cout << "Read Configuration of FMI Mapper" << std::endl;

	if (std::get_if<FMIInterfaceConfig>(&configVariants) == nullptr) {
		std::cerr << __FUNCTION__ << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	interfaceConfig = std::get<FMIInterfaceConfig>(configVariants);

	std::unique_ptr<fmi4cpp::fmi2::fmu> fmu(new fmi4cpp::fmi2::fmu(interfaceConfig.model));
	//CS = CoSimulation
	if (!fmu->supports_cs()) {
		std::cerr << __FUNCTION__ << "FMU contains no cs (CoSimulation) model" << std::endl;
		return 1;
	}

	// load co-simulation description from FMU
	auto coSimFMU = fmu->as_cs_fmu();
	auto modelDescription = coSimFMU->get_model_description();

	//cast iSimulationData to FMIBridge to move co-simulation FMU into it
	auto owner_tmp = owner.lock();
	auto bridge = (FMIBridge*) owner_tmp.get();
	if (!bridge) {
		std::cerr << __FUNCTION__ << "Incompatible owner, Not a FMIBridge" << std::endl;
		return 1;
	}
	bridge->coSimFMU = std::move(coSimFMU);

	for (const auto& var : *modelDescription->model_variables) {
		// cannot switch on outputVar.value_type because fmi4cpp uses strings and cannot use getType because type name 'Real' is not defined (maps to some floating point type, typically double) 
		if (fmi4cpp::fmi2::causality::input == var.causality || fmi4cpp::fmi2::causality::parameter == var.causality) {
			// FMI p56: Variables with causality = "parameter" or "input", as well as variables with variability = "constant", must have a "start" value.
			// return value 1 if "start" is missing
			if (var.is_boolean()) {
				auto boolVar = var.as_boolean();
				if (boolVar.start().has_value()) {
					config.boolInputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->bools.size()));
					state->bools.push_back(boolVar.start().value());
				}
				else {
					std::cerr << __FUNCTION__ << "boolean: start is missing" << std::endl;
					return 1;
				}
			}
			else if (var.is_enumeration() || var.is_integer()) {
				auto intVar = var.as_integer();
				if (intVar.start().has_value()) {
					config.intInputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->integers.size()));
					state->integers.push_back(intVar.start().value());
				}
				else {
					std::cerr << __FUNCTION__ << "integer or enum: start is missing" << std::endl;
					return 1;
				}
			}
			else if (var.is_real()) {
				auto realVar = var.as_real();
				if (realVar.start().has_value()) {
					if (typeid(fmi2Real) == typeid(double)) {
						config.doubleInputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->doubles.size()));
						state->doubles.push_back(realVar.start().value());

					}
					else {
						config.floatInputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->floats.size()));
						state->floats.push_back((float)realVar.start().value());
					}
				}
				else {
					std::cerr << __FUNCTION__ << "float: start is missing" << std::endl;
					return 1;
				}
			}
			else /*if (var.is_string())*/ {
				auto stringVar = var.as_string();
				if (stringVar.start().has_value()) {
					config.stringInputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->strings.size()));
					state->strings.push_back(stringVar.start().value());
					
				}
				else {
					std::cerr << __FUNCTION__ << "string: start is missing" << std::endl;
					return 1;
				}
			}
		}
		else if (fmi4cpp::fmi2::causality::output == var.causality || fmi4cpp::fmi2::causality::calculatedParameter == var.causality) {
			if (var.is_boolean()) {
				config.boolOutputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->bools.size()));
				state->bools.push_back(bool());
			}
			else if (var.is_enumeration() || var.is_integer()) {
				config.intOutputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->integers.size()));
				state->integers.push_back(int());
			}
			else if (var.is_real()) {
				if (typeid(fmi2Real) == typeid(double)) {
					config.doubleOutputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->doubles.size()));
					state->doubles.push_back(double());
				}
				else {
					config.floatOutputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->floats.size()));
					state->floats.push_back(float());
				}
			}
			else /*if (var.is_string())*/ {
				config.stringOutputList.push_back(NamesAndIndex(modelDescription->model_identifier + "." + var.name, var.name, (int)state->strings.size()));
				state->strings.push_back(std::string());
			}
		}
	}
	return 0;
}
