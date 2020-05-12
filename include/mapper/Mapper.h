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
enum eDataType
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
	std::map<std::string, interfaceNameAndIndex> intInputMap;
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

class iSimulationData;

/**
Abstract base class of all Mappers.
*/
class Mapper {

private:
	/**
	Configuration of Mapper.
	*/
	MapperConfig config;
protected:
	//most interfaces need these

	/**
	IP of interface. Used by almost all mapper implementations.
	*/
	std::string ip;
	/**
	Port of interface. Used by almost all mapper implementations.
	*/
	int port;
	/**
	This interface has this mapper
	*/
	std::weak_ptr<iSimulationData> owner;
public:
	/**
	Constructor of Mapper
	\param owner the owner of this mapper
	*/
	virtual void setOwner(std::shared_ptr<iSimulationData> owner) final;
	/**
	search input of this mapper from all other mappers
	\param simulationInterfaces all other simulation interfaces
	\return success status
	*/
	int searchInput(std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces);
	/**
	Read configuration and fill mapper configuration.
	\param config the decoding struct
	\return success status
	*/
	virtual int readConfiguration(configVariants_t config);
	/**
	Maps the given value, name and type to the interface.
	\param value value of the variable
	\param interfaceName name of variable in interface context
	\param type data type of variable
	*/
	void mapIn(values_t value, std::string interfaceName, eDataType type);

private:
	/**
	Checks if values of outputmapper are in the input. Maps the matches.
	\param outputMapper The mapper with the output.
	*/
	int filterInput(std::shared_ptr<Mapper> outputMapper);
	/**
	writes an error message
	\param base_name of variable that can not be filled by given outputs
	*/
	int writeInputError(std::string base_name);

protected:
	/**
	Maps the given value, name and type to the interface.
	\param value value of the variable
	\param interfaceName name of variable in interface context
	\param type data type of variable
	*/
	virtual void mapTo(values_t value, std::string interfaceName, eDataType type) = 0;
	
	/**
	Converts data type strings to enum values.
	\param type String representation of data type.
	\return Enum representation of data type.
	*/
	eDataType getType(std::string type);
};

#endif // !MAPPER_H
