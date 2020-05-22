#ifndef UNREALBRIDGE_H
#define UNREALBRIDGE_H

#include "simulation_interfaces/iSimulationData.h"
#include "../Mapper/Mapper.h"

class UnrealBridge : iSimulationData
{
public:
	UnrealBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int readOutputs() override;
	int doStep() override;

protected:
	void mapTo(values_t value, std::string interfaceName, eDataType type) override;
};

#endif // !UNREALBRIDGE_H