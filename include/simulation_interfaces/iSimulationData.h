#ifndef ISIMULATIONDATA_H
#define ISIMULATIONDATA_H

#include <vector>
#include <string>
#include <memory>
#include "../Mapper/Mapper.h"
#include "internalState.h"
/**
* Enum containing all supported interfaces and error for parsing failures.
*/
enum eSimulatorName {
	VTD,
	ROS,
	FMI,
	UNREAL,
	OSI,
	SUMO,

	SIMULATORNAME_ERROR
};

class Mapper;

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
	Updates all output data of each datatype in the internal state.
	\return Success status.
	*/
	//int readOutput();
	/**
	Search and map needed information of this interface from all other interfaces.
	\param simulatornInterfaces all interfaces
	\return Success status.
	*/
	int mapInput(std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces);
	/**
	Do simulation step.
	\return Success status.
	*/
	virtual int doStep() = 0;
	/**
	update outputs of the interface
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

};
#endif // !ISIMULATIONDATA_H