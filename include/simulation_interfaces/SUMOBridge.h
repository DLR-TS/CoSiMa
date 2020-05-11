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

	std::vector<int> updateInteger() override;
	std::vector<float> updateFloat() override;
	std::vector<double> updateDouble() override;
	std::vector<bool> updateBool() override;
	std::vector<std::string> updateString() override;

	int doStep() override;
};

#endif // !SUMOBRIDGE_H