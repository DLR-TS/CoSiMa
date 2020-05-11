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
	int doStep() override;

	std::vector<int> updateInteger() override;
	std::vector<float> updateFloat() override;
	std::vector<double> updateDouble() override;
	std::vector<bool> updateBool() override;
	std::vector<std::string> updateString() override;

};

#endif // !FMIBRIDGE_H