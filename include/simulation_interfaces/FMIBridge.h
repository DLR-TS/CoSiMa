#ifndef FMIBRIDGE_H
#define FMIBRIDGE_H

#include "iSimulationData.h"
#include "../Mapper/Mapper.h"
#include "../Mapper/FMIMapper.h"
//windef.h compatibility
#define NOMINMAX
#include "fmi4cpp/fmi4cpp.hpp"
//sleep to reduce poll interval for async FMUs
#include <thread>
#include <chrono>

/**
Class FMIBridge is a specialization of iSimulationData to access co-simulation FMUs as specified in the FMI 2.0 Standard

<\br>
Current implementation is incapable of setting initial approximations of calculated parameters because FMIBridge::init 
has to step too far through the FMUs state machine for the initialization, leaving the FMU in InitializationMode unto 
the first call of FMIBridge::doStep.
*/
class FMIBridge : public iSimulationData
{
	friend class FMIMapper;
private:
	std::unique_ptr<fmi4cpp::fmi2::cs_fmu> coSimFMU;
	std::shared_ptr<fmi4cpp::fmi2::cs_slave> coSimSlave;
	bool enteredInitializationMode;
	bool leftInitializationMode;


	class FMUSlaveStateWrapper {
	private:
		//cs_slave creating this fmu state is needed later for freeing the memory again
		std::shared_ptr<fmi4cpp::fmi2::cs_slave> coSimSlave;

		FMUSlaveStateWrapper(std::shared_ptr < fmi4cpp::fmi2::cs_slave> slave);

	public:
		~FMUSlaveStateWrapper();

		fmi4cpp::fmi4cppFMUstate state;
		static std::optional<FMUSlaveStateWrapper> tryGetStateOf(std::shared_ptr<fmi4cpp::fmi2::cs_slave> slave);
	};

public:
	FMIBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper){};

	/**
	Initialize the interface.
	<\br>
	Creates a new FMU instance and sets up the experiment with the given start time.
	<\br>
	Leaves the FMU in InitializationMode until the first call of FMIBridge::doStep. Thus, initial approximations of calculatedParameters cannot be set.

	\param scenario Scenario identification. -- Currently ignored because FMI4cpp autogenerates instance names
	\param starttime Time of start.
	\param mode The mode to start in.
	\return Success status.
	*/
	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string info) override;
	int disconnect() override;

	int writeToInternalState() override;
	/**
	Reads the internal state into the simulation interface.
	<\br>
	Ignores values that cannot be set in the FMUs current state (see FMI Standard, especially section 2.2.7)
	*/
	int readFromInternalState() override;

	/**
	Do simulation step.<\br>
	First call ends InitializationMode of FMU.
	\return Success status.
	*/
	int doStep(double stepSize = 1) override;
};

#endif // !FMIBRIDGE_H