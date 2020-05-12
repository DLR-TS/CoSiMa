#ifndef SUMOBRIDGE_H
#define SUMOBRIDGE_H

#include "iSimulationData.h"
#include "../Mapper/Mapper.h"

class SUMOBridge : iSimulationData
{
public:
	SUMOBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int readOutputs() override;
	int doStep() override;
};

#endif // !SUMOBRIDGE_H