#ifndef ISIMULATIONDATA_H
#define ISIMULATIONDATA_H

#include <vector>
#include <string>
#include <memory>
#include "base_interfaces/BaseSystemInterface.h"
#include "Mapper/Mapper.h"
#include "internalState.h"

//forward declarations
/**
* Basic value types
*/
typedef std::variant<int, float, double, bool, std::string> values_t;
enum eDataType;
class BaseSystemInterface;
class Mapper;

/**
* Enum containing all supported interfaces and error for parsing failures.
*/
enum eSimulatorName {
	FMI, //needs to be first
	VTD,
	ROS,
	UNREAL,
	OSI,
	SUMO,

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
	iSimulationData(std::shared_ptr<Mapper> mapper) {
		this->mapper = mapper;
		this->state = std::make_shared<internalState>();
	}
protected:
	/**
	Holds all input variables.
	*/
	std::shared_ptr<internalState> state;
	/**
	Specific mapper of this interface.
	*/
	std::shared_ptr<Mapper> mapper;

public:
	/**
	Initialize the interface.
	\param scenario Scenario identification.
	\param starttime Time of start.
	\param mode The mode to start in.
	\return Success status.
	*/
	virtual int init(std::string scenario, float starttime, int mode) = 0;
	/**
	Connect with followed information.
	\param info Information to connect with simulator.
	\return Success status.
	*/
	virtual int connect(std::string info) = 0;
	/**
	Disconnect from interface.
	\return Success status.
	*/
	virtual int disconnect() = 0;
	/**
	Search and map needed information of this interface from all other interfaces.
	\param baseInterface base interface
	\return Success status.
	*/
	int mapInput(std::shared_ptr<BaseSystemInterface> baseInterface);
	/**
	Write output data of interface to base system
	\return Success status.
	*/
	int writeTo(std::shared_ptr<BaseSystemInterface> baseInterface);
	/**
	Do simulation step.
	\return Success status.
	*/
	virtual int doStep() = 0;
	/**
	update outputs of the interface
	use the Mapper::mapIn method to load Outputs
	*/
	virtual int readOutputs() = 0;
	/**
	\return Mapper of this interface.
	*/
	std::shared_ptr<Mapper> getMapper();
public:
	/**
	\return Output variables of the interface.
	*/
	const std::shared_ptr<internalState> getInternalState();

protected:
	/**
	Maps the given value, name and type to the interface.
	Called by Mapper::mapTo because the interface context is required, which is not known to the mapper.
	\param value value of the variable
	\param interfaceName name of variable in interface context
	\param type data type of variable
	*/
	virtual void mapTo(values_t value, std::string interfaceName, eDataType type) = 0;
	////friend Mapper::mapTo to allow access to mapTo for delegation
	//friend void Mapper::mapTo(values_t value, std::string interfaceName, eDataType type);
	// friendship cannot be reduced to single member function because it is not in the forward decleration and thus unknown
	friend class Mapper;

};
#endif // !ISIMULATIONDATA_H