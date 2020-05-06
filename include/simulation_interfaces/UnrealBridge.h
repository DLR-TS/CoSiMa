#ifndef UNREALBRIDGE_H
#define UNREALBRIDGE_H

#include "simulation_interfaces/iSimulationData.h"
#include "../Mapper/Mapper.h"

class UnrealBridge : iSimulationData
{
public:
	UnrealBridge(Mapper* mapper) : iSimulationData(mapper) {};

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

#endif // !UNREALBRIDGE_H