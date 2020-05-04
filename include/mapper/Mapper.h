#ifndef MAPPER_H
#define MAPPER_H

#include <cctype>
#include <string>
#include <map>
#include <variant>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "simulation_interfaces/iSimulationData.h"
#include "simulation_interfaces/internalState.h"
#include "configreader/StandardYAMLConfig.h"

typedef std::variant<int, float, double, bool, std::string> values_t;

enum DataTypes
{
	INTEGER,
	FLOAT,
	DOUBLE,
	BOOL,
	STRING,
	ERROR
};

struct interfaceNameAndIndex {
	std::string interfaceName;
	int index;
};

class MapperConfig {
public:
	//base_names to interface_names
	std::map < std::string, interfaceNameAndIndex> intInputMap;
	std::map<std::string, interfaceNameAndIndex> intOutputMap;
	std::map<std::string, interfaceNameAndIndex> floatInputMap;
	std::map<std::string, interfaceNameAndIndex> floatOutputMap;
	std::map<std::string, interfaceNameAndIndex> doubleInputMap;
	std::map<std::string, interfaceNameAndIndex> doubleOutputMap;
	std::map<std::string, interfaceNameAndIndex> boolInputMap;
	std::map<std::string, interfaceNameAndIndex> boolOutputMap;
	std::map<std::string, interfaceNameAndIndex> stringInputMap;
	std::map<std::string, interfaceNameAndIndex> stringOutputMap;
};

class Mapper {

private:
	MapperConfig config;
protected:
	//most interfaces need these
	std::string ip;
	int port;

public:
	int extractInternalState(internalState* pointer);
	int readConfiguration(InterfaceYAMLConfig config);

private:
	void filterInput(Mapper* outputMapper, internalState* state);

protected:
	virtual void mapTo(values_t value, std::string name, DataTypes type) = 0;
	DataTypes getType(std::string);
};

#endif // !MAPPER_H
