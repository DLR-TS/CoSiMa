#include "mapper/Mapper.h"
#include "CoSiMa.h"

int Mapper::searchInput(std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces) {
	for (auto &simulationInterface : simulationInterfaces)
	{
		std::shared_ptr<Mapper> outputMapper = simulationInterface->getMapper();
		if (filterInput(outputMapper)) {
			return 1;
		}
	}
	return 0;
}

int Mapper::filterInput(std::shared_ptr<Mapper> outputMapper) {
	//integer
	for (std::pair<std::string, interfaceNameAndIndex> input : config.intInputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.intOutputMap) {
			if (input.first == output.first) {
				//each Mapper implementation handles this
				int value = outputMapper->owner.lock()->getInternalState()->integers.at(output.second.index);
				mapTo(value, input.second.interfaceName, INTEGER);
			}
		}
	}
	//float
	for (std::pair<std::string, interfaceNameAndIndex> input : config.floatInputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.floatOutputMap) {
			if (input.first == output.first) {
				//each Mapper implementation handles this
				float value = outputMapper->owner.lock()->getInternalState()->floats.at(output.second.index);
				mapTo(value, input.second.interfaceName, FLOAT);
				break;
			}
		}
	}
	//double
	for (std::pair<std::string, interfaceNameAndIndex> input : config.doubleInputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.doubleOutputMap) {
			if (input.first == output.first) {
				//each Mapper implementation handles this
				double value = outputMapper->owner.lock()->getInternalState()->doubles.at(output.second.index);
				mapTo(value, input.second.interfaceName, DOUBLE);
				break;
			}
		}
	}
	//bool
	for (std::pair<std::string, interfaceNameAndIndex> input : config.boolInputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.boolOutputMap) {
			if (input.first == output.first) {
				//each Mapper implementation handles this
				bool value = outputMapper->owner.lock()->getInternalState()->bools.at(output.second.index);
				mapTo(value, input.second.interfaceName, BOOL);
				break;
			}
		}
	}
	//std::string
	for (std::pair<std::string, interfaceNameAndIndex> input : config.stringInputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.stringOutputMap) {
			if (input.first == output.first) {
				//each Mapper implementation handles this
				std::string value = outputMapper->owner.lock()->getInternalState()->strings.at(output.second.index);
				mapTo(value, input.second.interfaceName, STRING);
				break;
			}
		}
	}
	return 0;
}

int Mapper::writeInputError(std::string base_name) {
	std::cout << "Input of variable could not be matched: base_name: " << base_name << std::endl;
	return 1;
}

int Mapper::readConfiguration(configVariants_t configVariants) {

	if (std::get_if<InterfaceYAMLConfig>(&configVariants) == nullptr) {
		std::cout << "Wrong Configuration! Implement specific readConfiguration method for this interface." << std::endl;
		return 1;
	}
	InterfaceYAMLConfig yamlconfig = std::get<InterfaceYAMLConfig>(configVariants);

	port = yamlconfig.port;
	ip = yamlconfig.ip;

	//fill input and output vectors
	for (VariableDefinitionMap definition : yamlconfig.input_map) {
		interfaceNameAndIndex info;
		switch (getType(definition.type)) {
		case BOOL:
			info.index = (int) config.boolInputMap.size();
			info.interfaceName = definition.interface_name;
			config.boolInputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
			break;
		case INTEGER:
			info.index = (int) config.intInputMap.size();
			info.interfaceName = definition.interface_name;
			config.intInputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
			break;
		case FLOAT:
			info.index = (int) config.floatInputMap.size();
			info.interfaceName = definition.interface_name;
			config.floatInputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
			break;
		case DOUBLE:
			info.index = (int) config.doubleInputMap.size();
			info.interfaceName = definition.interface_name;
			config.doubleInputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
			break;
		case STRING:
			info.index = (int) config.stringInputMap.size();
			info.interfaceName = definition.interface_name;
			config.stringInputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
			break;
		case DATATYPE_ERROR:
			std::cout << "Wrong definition of input_map. Allowed are: string, int, (integer), float, double, bool, (boolean)" << std::endl;
			return 1;
		}

		for (VariableDefinitionMap definition : yamlconfig.output_map) {
			interfaceNameAndIndex info;
			switch (getType(definition.type)) {
			case BOOL:
				info.index = (int) config.boolOutputMap.size();
				info.interfaceName = definition.interface_name;
				config.boolOutputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
				break;
			case INTEGER:
				info.index = (int) config.intOutputMap.size();
				info.interfaceName = definition.interface_name;
				config.intOutputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
				break;
			case FLOAT:
				info.index = (int) config.floatOutputMap.size();
				info.interfaceName = definition.interface_name;
				config.floatOutputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
				break;
			case DOUBLE:
				info.index = (int) config.doubleOutputMap.size();
				info.interfaceName = definition.interface_name;
				config.doubleOutputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
				break;
			case STRING:
				info.index = (int) config.stringOutputMap.size();
				info.interfaceName = definition.interface_name;
				config.stringOutputMap.insert(std::pair<std::string, interfaceNameAndIndex>(definition.base_name, info));
				break;
			case DATATYPE_ERROR:
				std::cout << "Wrong definition of type in output_map. Allowed are: string, int, (integer), float, double, bool, (boolean)" << std::endl;
				return 1;
			}
		}
	}
	return 0;
}

void Mapper::mapIn(values_t value, std::string interfaceName, eDataType type) {
	switch(type) {
	case BOOL:
		for (auto const& entry : config.boolOutputMap)
		{
			if (entry.second.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->bools.at(entry.second.index) = std::get<bool>(value);
				break;
			}
		}
	case INTEGER:
		for (auto const& entry : config.intOutputMap)
		{
			if (entry.second.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->integers.at(entry.second.index) = std::get<int>(value);
				break;
			}
		}
	case FLOAT:
		for (auto const& entry : config.floatOutputMap)
		{
			if (entry.second.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->floats.at(entry.second.index) = std::get<float>(value);
				break;
			}
		}
	case DOUBLE:
		for (auto const& entry : config.doubleOutputMap)
		{
			if (entry.second.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->doubles.at(entry.second.index) = std::get<double>(value);
				break;
			}
		}
	case STRING:
		for (auto const& entry : config.stringOutputMap)
		{
			if (entry.second.interfaceName == interfaceName) {
				owner.lock()->getInternalState()->strings.at(entry.second.index) = std::get<std::string>(value);
				break;
			}
		}
	}
}

eDataType Mapper::getType(std::string type) {
	std::transform(type.begin(), type.end(), type.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (type.compare("string") == 0) {
		return STRING;
	}
	else if (type.compare("float") == 0) {
		return FLOAT;
	}
	else if (type.compare("double") == 0) {
		return DOUBLE;
	}
	else if (type.compare("bool") == 0 || type.compare("boolean") == 0) {
		return BOOL;
	}
	else if (type.compare("int") == 0 || type.compare("integer") == 0) {
		return INTEGER;
	}
	else {
		return DATATYPE_ERROR;
	}
}

void Mapper::setOwner(std::shared_ptr<iSimulationData> owner) {
	this->owner = owner;
}
