#include "mapper/Mapper.h"
#include "CoSiMa.h"

int Mapper::searchInput(std::shared_ptr<BaseSystemInterface> baseInterface) {
	//integer
	for (const NamesAndIndex &input : config.intInputList) {
		int value = baseInterface->getIntValue(input.baseName);
		mapTo(value, input.interfaceName, INTEGERCOSIMA);
	}
	//float
	for (const NamesAndIndex &input : config.floatInputList) {
		float value = baseInterface->getFloatValue(input.baseName);
		mapTo(value, input.interfaceName, FLOATCOSIMA);
	}
	//double
	for (const NamesAndIndex &input : config.doubleInputList) {
		double value = baseInterface->getDoubleValue(input.baseName);
		mapTo(value , input.interfaceName, DOUBLECOSIMA);
	}
	//bool
	for (const NamesAndIndex &input : config.boolInputList) {
		bool value = baseInterface->getBoolValue(input.baseName);
		mapTo(value, input.interfaceName, BOOLCOSIMA);
	}
	//std::string
	for (const NamesAndIndex &input : config.stringInputList) {
		std::string value = baseInterface->getStringValue(input.baseName);
		mapTo(value, input.interfaceName, STRINGCOSIMA);
	}
	return 0;
}

int Mapper::writeOutput(std::shared_ptr<BaseSystemInterface> baseInterface) {
	//integer
	for (const NamesAndIndex &output : config.intOutputList) {
		baseInterface->setIntValue(output.baseName, owner.lock()->getInternalState()->integers.at(output.index));
	}
	//float
	for (const NamesAndIndex &output : config.floatOutputList) {
		baseInterface->setFloatValue(output.baseName, owner.lock()->getInternalState()->floats.at(output.index));
	}
	//double
	for (const NamesAndIndex &output : config.doubleOutputList) {
		baseInterface->setDoubleValue(output.baseName, owner.lock()->getInternalState()->doubles.at(output.index));
	}
	//bool
	for (const NamesAndIndex &output : config.boolOutputList) {
		baseInterface->setBoolValue(output.baseName, owner.lock()->getInternalState()->bools.at(output.index));
	}
	//std::string
	for (const NamesAndIndex &output : config.stringOutputList) {
		baseInterface->setStringValue(output.baseName, owner.lock()->getInternalState()->strings.at(output.index));
	}
	return 0;
}

int Mapper::readConfiguration(configVariants_t configVariants) {

	if (std::get_if<InterfaceYAMLConfig>(&configVariants) == nullptr) {
		std::cout << "Wrong Configuration! Implement specific readConfiguration method for this interface." << std::endl;
		return 1;
	}
	InterfaceYAMLConfig yamlconfig = std::get<InterfaceYAMLConfig>(configVariants);

	port = yamlconfig.port;
	ip = yamlconfig.ip;

	//fill input vectors
	for (VariableDefinition definition : yamlconfig.inputs) {

		switch (getType(definition.type)) {
		case BOOLCOSIMA:
			config.boolInputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case INTEGERCOSIMA:
			config.intInputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case FLOATCOSIMA:
			config.floatInputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case DOUBLECOSIMA:
			config.doubleInputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case STRINGCOSIMA:
			config.stringInputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case DATATYPE_ERROR_COSIMA:
			std::cout << "Wrong definition of input_map. Allowed are: string, int, (integer), float, double, bool, (boolean)" << std::endl;
			return 1;
		}
	}
	//fill output vectors
	for (VariableDefinition definition : yamlconfig.outputs) {
		switch (getType(definition.type)) {
		case BOOLCOSIMA:
			config.boolOutputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case INTEGERCOSIMA:
			config.intOutputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case FLOATCOSIMA:
			config.floatOutputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case DOUBLECOSIMA:
			config.doubleOutputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case STRINGCOSIMA:
			config.stringOutputList.push_back(NamesAndIndex(definition.base_name, definition.interface_name, (int)config.boolInputList.size()));
			break;
		case DATATYPE_ERROR_COSIMA:
			std::cout << "Wrong definition of type in output_map. Allowed are: string, int, (integer), float, double, bool, (boolean)" << std::endl;
			return 1;
		}
	}
	return 0;
}

void Mapper::mapIn(values_t value, std::string interfaceName, eDataType type) {
	switch(type) {
	case BOOLCOSIMA:
		for (NamesAndIndex const &entry : config.boolOutputList)
		{
			if (entry.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->bools.at(entry.index) = std::get<bool>(value);
				break;
			}
		}
	case INTEGERCOSIMA:
		for (NamesAndIndex const &entry : config.intOutputList)
		{
			if (entry.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->integers.at(entry.index) = std::get<int>(value);
				break;
			}
		}
	case FLOATCOSIMA:
		for (NamesAndIndex const &entry : config.floatOutputList)
		{
			if (entry.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->floats.at(entry.index) = std::get<float>(value);
				break;
			}
		}
	case DOUBLECOSIMA:
		for (NamesAndIndex const &entry : config.doubleOutputList)
		{
			if (entry.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->doubles.at(entry.index) = std::get<double>(value);
				break;
			}
		}
	case STRINGCOSIMA:
		for (NamesAndIndex const &entry: config.stringOutputList)
		{
			if (entry.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->strings.at(entry.index) = std::get<std::string>(value);
				break;
			}
		}
	}
}

void Mapper::mapTo(values_t value, std::string interfaceName, eDataType type) {
	owner.lock()->mapTo(value, interfaceName, type);
}

eDataType Mapper::getType(std::string type) {
	std::transform(type.begin(), type.end(), type.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (type.compare("string") == 0) {
		return STRINGCOSIMA;
	}
	else if (type.compare("float") == 0) {
		return FLOATCOSIMA;
	}
	else if (type.compare("double") == 0) {
		return DOUBLECOSIMA;
	}
	else if (type.compare("bool") == 0 || type.compare("boolean") == 0) {
		return BOOLCOSIMA;
	}
	else if (type.compare("int") == 0 || type.compare("integer") == 0) {
		return INTEGERCOSIMA;
	}
	else {
		return DATATYPE_ERROR_COSIMA;
	}
}

void Mapper::setOwner(std::shared_ptr<iSimulationData> owner) {
	this->owner = owner;
}
