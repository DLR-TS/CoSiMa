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

	std::vector<int> updateInteger() override;
	std::vector<float> updateFloat() override;
	std::vector<double> updateDouble() override;
	std::vector<bool> updateBool() override;
	std::vector<std::string> updateString() override;

	int doStep() override;
};

#endif // !ROSBRIDGE_H