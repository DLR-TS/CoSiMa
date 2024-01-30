/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef MAPPER_H
#define MAPPER_H

#include <cctype>
#include <string>

#if __has_include(<variant>)
	#include <variant>
#elif __has_include("boost/variant.hpp")
	#include "boost/variant.hpp"
#else
error "Missing the <variant> header."
#endif
#include <iostream>
#include <algorithm>

#include "base_interfaces/BaseSystemInterface.h"
#include "simulation_interfaces/iSimulationData.h"
#include "reader/StandardYAMLConfig.h"

//forward declaration
class BaseSystemInterface;

/**
* YAML configuration structs
*/
#if __has_include(<variant>)
	typedef std::variant<InterfaceYAMLConfig, OSMPInterfaceConfig> configVariants_t;
#elif __has_include("boost/variant.hpp")
	typedef boost::variant<InterfaceYAMLConfig, OSMPInterfaceConfig> configVariants_t;
#endif

struct AnnotatedMessage {
	std::string message;
	std::string base_name;
	std::string interface_name;
};

AnnotatedMessage convertToAnnotatedMessage(ConfigParameter& definition);

/**
* Configuration of Mapper.
* Contains the relation between base name and (interface name and index in data structure) for each datatype as well as input and output.
*/
struct Data {
	//collection of inputs
	std::list<AnnotatedMessage> messageInputList{};
	//collection of outputs
	std::list<AnnotatedMessage> messageOutputList{};
};

class SimulatorInterface;

/**
Abstract base class of all Mappers.
*/
class Mapper {

public:
	Mapper() {
		//this->messageCache = std::make_shared<MessageCache>();
	}

protected:
	/**
	Configuration of Mapper.
	*/
	Data data;

	/**
	This interface contains this mapper
	*/
	std::weak_ptr<SimulatorInterface> owner;
public:
	/**
	Read configuration and fill mapper configuration.
	\param config the decoding struct
	\return valid status
	*/
	virtual int readConfiguration(configVariants_t config);
	/**
	Constructor of Mapper
	\param owner the owner of this mapper
	*/
	virtual void setOwner(std::weak_ptr<SimulatorInterface> owner) final;
	/**
	search input of this mapper from base system interface
	\param simulationInterface simulation system interfaces
	*/
	void searchInput(std::shared_ptr<BaseSystemInterface> simulationInterface);
	/**
	write output variables to base interface
	\param baseInterface base system simulation interfaces
	*/
	void writeOutput(std::shared_ptr<BaseSystemInterface> baseInterface);
	/**
	Maps the given value, name and type to the internalState
	\param value OSI message to map
	\param interfaceName name of variable in interface context
	*/
	void mapToInternalState(std::string& value, std::string& interfaceName);
	/**
	Retrieves the value mapped to the given name and type from the internalState
	\param interfaceName name of the variable in interface context
	\return mapped value in internalState
	*/
	std::string mapFromInternalState(std::string& interfaceName);
};

#endif // !MAPPER_H
