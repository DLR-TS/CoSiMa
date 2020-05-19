#ifndef FMIBRIDGE_H
#define FMIBRIDGE_H

#include "iSimulationData.h"
#include "../Mapper/Mapper.h"

class FMIBridge : iSimulationData
{
public:
	FMIBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int readOutputs() override;
	int doStep(double stepSize = 1) override;
};

#endif // !FMIBRIDGE_H