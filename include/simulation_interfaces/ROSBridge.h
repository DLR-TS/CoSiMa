#ifndef ROSBRIDGE_H
#define ROSBRIDGE_H

#include "simulation_interfaces/iSimulationData.h"
#include "../Mapper/Mapper.h"

class ROSBridge : iSimulationData
{
public:
	ROSBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int readOutputs() override;
	int doStep() override;
};

#endif // !ROSBRIDGE_H