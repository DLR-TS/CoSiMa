/**
@authors German Aerospace Center: Bj�rn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef ISIMULATIONDATA_H
#define ISIMULATIONDATA_H

#include <vector>
#include <string>
#include <memory>
#include "base_interfaces/BaseSystemInterface.h"
#include "mapper/Mapper.h"
#include "internalState.h"
#include "reader/StandardYAMLConfig.h"

//forward declarations
/**
* Basic value types
*/
typedef std::variant<int, float, double, bool, std::string> values_t;
typedef std::variant<InterfaceYAMLConfig, FMIInterfaceConfig, OSIInterfaceConfig, OSMPInterfaceConfig> configVariants_t;
class BaseSystemInterface;
class Mapper;

/**
* Enum containing all supported interfaces and error for parsing failures.
*/
enum eSimulatorName
{
	FMI, //needs to be first
	OSI,
	OSMP,
	CARLA,
	DEFAULT,

	SIMULATORNAME_ERROR //needs to be last
};

/**
Abstract class for all simulation interfaces.
*/
class iSimulationData
{
public:
	/**
	Constructor of iSimulationData.
	\param mapper Mapper to be set.
	*/
	iSimulationData(std::shared_ptr<Mapper> mapper, bool verbose)
	{
		this->mapper = mapper;
		this->verbose = verbose;
	}

protected:
	/**
	Holds a copy of the simulator interface variables.
	*/
	std::shared_ptr<internalState> state;
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
	Initialize the interface.
	\param starttime Time of start.
	\return Success status.
	*/
	virtual int init(float starttime = 0.0) = 0;
	/**
	Disconnect from interface.
	\return Success status.
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
	update outputs of the interface in the internal state
	uses the Mapper::mapToInternalState method to write outputs
	*/
	virtual int writeToInternalState() = 0;
	/**
	\return Mapper of this interface.
	*/
	std::shared_ptr<Mapper> getMapper();

	/**
	Reads the internal state into the simulation interface.
	*/
	virtual int readFromInternalState() = 0;

	/**
	Read configuration and fill simulation configuration.
	\param config the decoding struct
	\return success status
	*/
	virtual int readConfiguration(configVariants_t configVariants);
};
#endif // !ISIMULATIONDATA_H
