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
enum SupportedInterfaces {
	VTD,
	ROS,
	FMI,
	UNREAL,
	OSI,
	SUMO,

	SUPPORTEDINTERFACES_ERROR
};

class Mapper;

/**
* Abstract class for all simulation interfaces.
*/
class iSimulationData
{
public:
	/**
	* Constructor of iSimulationData.
	* \param mapper Mapper to be set.
	*/
	iSimulationData(std::shared_ptr<Mapper> mapper) {
		this->mapper = mapper;
	}
protected:
	/**
	* Holds all input variables.
	*/
	std::shared_ptr<internalState> state;
	/**
	* Specific mapper of this interface.
	*/
	std::shared_ptr<Mapper> mapper;

public:
	/**
	* Initialize the interface.
	* \param scenario Scenario identification.
	* \param starttime Time of start.
	* \param mode The mode to start in.
	* \return Success status.
	*/
	virtual int init(std::string scenario, float starttime, int mode) = 0;
	/**
	* Connect with followed information.
	* \param info Information to connect with simulator.
	* \return Success status.
	*/
	virtual int connect(std::string info) = 0;
	/**
	* Disconnect from interface.
	* \return Success status.
	*/
	virtual int disconnect() = 0;
	/**
	* Updates all data of each datatype in the internal state.
	* \return Success status.
	*/
	int update();
	/**
	* Map all availible information of this interface to all other interfaces.
	* \return Success status.
	*/
	int mapToOtherInterfaces();
	/**
	* Do simulation step.
	* \return Success status.
	*/
	virtual int doStep() = 0;
	/**
	* \return Mapper of this interface.
	*/
	std::shared_ptr<Mapper> getMapper();

protected:
	/**
	* \return Vector of udpated interface output integer values.
	*/
	virtual std::vector<int> updateInteger() = 0;
	/**
	* \return Vector of updated interface output float values.
	*/
	virtual std::vector<float> updateFloat() = 0;
	/**
	* \return Vector of updated interface output double values.
	*/
	virtual std::vector<double> updateDouble() = 0;
	/**
	* \return Vector of updated interface output bool values.
	*/
	virtual std::vector<bool> updateBool() = 0;
	/**
	* \return Vector of updated interface output string values.
	*/
	virtual std::vector<std::string> updateString() = 0;
public:
	/**
	* \return Output variables of the interface.
	*/
	const std::shared_ptr<internalState> getInternalState();

public:
// not needed
/*	virtual void setInteger(std::vector<int> ints) = 0;
	virtual void setFloat(std::vector<float> floats) = 0;
	virtual void setDouble(std::vector<double> doubles) = 0;
	virtual void setBool(std::vector<bool> bools) = 0;
	virtual void setString(std::vector<std::string> strings) = 0;*/

};
#endif // !ISIMULATIONDATA_H