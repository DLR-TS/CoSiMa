#ifndef MAPPER_H
#define MAPPER_H

#include <cctype>
#include <string>
#include <map>
#include <variant>
#include <iostream>
#include "simulation_interfaces/iSimulationData.h"
#include "simulation_interfaces/internalState.h"
#include "configreader/StandardYAMLConfig.h"

/**
* Basic value types
*/
typedef std::variant<int, float, double, bool, std::string> values_t;
/**
* YAML configuration structs
*/
typedef std::variant<InterfaceYAMLConfig, FMIInterfaceConfig> configVariants_t;

/**
* Basic data types enum
*/
enum DataTypes
{
	INTEGER,
	FLOAT,
	DOUBLE,
	BOOL,
	STRING,
	DATATYPE_ERROR
};

/**
* Contains information about variable interface name and its position in vector.
* \var std::string interfaceName
* holds the interface name of variable
* \var int index
* holds the index where the variable with the name is located
*/
struct interfaceNameAndIndex {
	std::string interfaceName;
	int index;
};

/**
* Configuration of Mapper.
* Contains the relation between base name and (interface name and index in data structure) for each datatype as well as input and output. 
*/
class MapperConfig {
public:
	//base_names to interface_names relation
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


/**
* Abstract base class of all Mappers.
*/
class Mapper {

private:
	/**
	* Configuration of Mapper.
	*/
	MapperConfig config;
protected:
	//most interfaces need these

	/**
	* IP of interface. Used by almost all mapper implementations.
	*/
	std::string ip;
	/**
	* Port of interface. Used by almost all mapper implementations.
	*/
	int port;

public:
	/**
	* send values of internal states to other mappers (their interfaces).
	* \param state own state
	* \return success status
	*/
	int extractInternalState(std::shared_ptr<internalState> state);
	/**
	* Read configuration and fill mapper configuration.
	* \return success status
	*/
	int readConfiguration(configVariants_t config);

private:
	/**
	* Checks if values of outputmapper are in the input. Maps the matches.
	* \param outputMapper The mapper with the output.
	* \param state internal state of interface of output mapper to read values from
	*/
	void filterInput(Mapper* outputMapper, std::shared_ptr<internalState> state);

protected:
	/**
	* Maps the given value, name and type to the interface.
	* \param value value of the variable
	* \param interfaceName name of variable in interface context
	* \param type data type of variable
	*/
	virtual void mapTo(values_t value, std::string interfaceName, DataTypes type) = 0;
	
	/**
	* Converts data type strings to enum values.
	* \param type String representation of data type.
	* \return Enum representation of data type.
	*/
	DataTypes getType(std::string type);
public:
	~Mapper() {};
};

#endif // !MAPPER_H
