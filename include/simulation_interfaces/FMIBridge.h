#ifndef FMIBRIDGE_H
#define FMIBRIDGE_H

#include "iSimulationData.h"
#include "../Mapper/Mapper.h"
#include "fmi4cpp/fmi4cpp.hpp"

class FMIBridge : iSimulationData
{
private:
	std::unique_ptr<fmi4cpp::fmi2::cs_fmu> coSimFMU;
	std::unique_ptr<fmi4cpp::fmi2::cs_slave> coSimFMUInstance;

public:
	FMIBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int readOutputs() override;
	int doStep(double stepSize = 1) override;
};

#endif // !FMIBRIDGE_H