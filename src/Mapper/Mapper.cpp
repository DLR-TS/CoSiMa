#include "mapper/Mapper.h"
#include <string>
#include "CoSiMa.h"

//output part
int Mapper::extractInternalState(internalState* state)
{
	for (auto simulationInterface : simulationInterfaces)
	{
		Mapper* inputMapper = simulationInterface->getMapper();
		inputMapper->filterInput(this, state);
	}
	return 0;
}

//called from output part of Mapper
void Mapper::filterInput(Mapper* outputMapper, internalState* foreignState) {
	//integer
	for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.intOutputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> input : config.intInputMap) {
			if (input.first.compare(output.first)) {
				//each Mapper implementation handles this
				mapTo(foreignState->integers.at(output.second.index), input.second.interfaceName, INTEGER);
			}
		}
	}
	//float
	for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.floatOutputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> input : config.floatInputMap) {
			if (input.first.compare(output.first)) {
				//each Mapper implementation handles this
				mapTo(foreignState->floats.at(output.second.index), input.second.interfaceName, FLOAT);
			}
		}
	}
	//double
	for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.doubleOutputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> input : config.doubleInputMap) {
			if (input.first.compare(output.first)) {
				//each Mapper implementation handles this
				mapTo(foreignState->doubles.at(output.second.index), input.second.interfaceName, DOUBLE);
			}
		}
	}
	//bool
	for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.boolOutputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> input : config.boolInputMap) {
			if (input.first.compare(output.first)) {
				//each Mapper implementation handles this
				mapTo(foreignState->bools.at(output.second.index), input.second.interfaceName, BOOL);
			}
		}
	}
	//string
	for (std::pair<std::string, interfaceNameAndIndex> output : outputMapper->config.stringOutputMap) {
		for (std::pair<std::string, interfaceNameAndIndex> input : config.stringInputMap) {
			if (input.first.compare(output.first)) {
				//each Mapper implementation handles this
				mapTo(foreignState->strings.at(output.second.index), input.second.interfaceName, STRING);
			}
		}
	}
}

int Mapper::readConfiguration(InterfaceYAMLConfig yamlconfig) {


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
		case ERROR:
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
			case ERROR:
				std::cout << "Wrong definition of type in output_map. Allowed are: string, int, (integer), float, double, bool, (boolean)" << std::endl;
				return 1;
			}
		}
	}
	return 0;
}

DataTypes Mapper::getType(std::string type) {
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
		return ERROR;
	}
}
