/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef ISIMULATIONDATA_H
#define ISIMULATIONDATA_H

#include <vector>
#include <string>
#include <memory>
#include "base_interfaces/BaseSystemInterface.h"
#include "mapper/Mapper.h"
#include "reader/StandardYAMLConfig.h"

//forward declarations
/**
* Basic value types
*/
//typedef std::variant<int, float, double, bool, std::string> values_t;
typedef std::variant<InterfaceYAMLConfig, OSMPInterfaceConfig> configVariants_t;
class BaseSystemInterface;
class Mapper;

/**
* Enum containing all supported interfaces and error for parsing failures.
*/
enum eSimulatorTypes
{
	CARLA,
	OSMP,
	DUMMY,

	SIMULATORNAME_ERROR //needs to be last
};

/**
Abstract class for all simulation interfaces.
*/
class SimulatorInterface : public std::enable_shared_from_this<SimulatorInterface>
{
protected:
	/**
	Specific mapper of this interface.
	*/
	std::shared_ptr<Mapper> mapper;
	/**
	* log level
	*/
	bool verbose = false;

public:
	/**
	Read configuration and fill simulation configuration.
	\param node yaml node
	\return valid status
	*/
	virtual void configure(YAML::Node node);
	/**
	Initialize the interface.
	\param verbose enable verbose output
	\return success status.
	*/
	virtual int init(bool verbose) = 0;
	/**
	Disconnect from interface.
	\return success status
	*/
	virtual int disconnect() = 0;
	/**
	Search and map needed information of this interface from all other interfaces.
	\param baseInterface base interface
	*/
	void mapToInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface);
	/**
	Write output data of interface to base system
	*/
	void mapFromInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface);
	/**
	Do simulation step.
	\return Success status.
	*/
	virtual int doStep(double stepSize = 1) = 0;
	/**
	update outputs of the interface in the internal messageCache
	uses the Mapper::mapToInternalState method to write outputs
	*/
	virtual int writeToInternalState() = 0;

	/**
	Reads the internal messageCache into the simulation interface.
	*/
	virtual int readFromInternalState() = 0;

	/**
	Configure mapper weak pointer
	*/
	void configureMapper();

	/**
	Stop the simulation run gracefully.
	*/
	virtual void stopSimulation() {};
};
#endif // !ISIMULATIONDATA_H
