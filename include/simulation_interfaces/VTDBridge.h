#ifndef VTDBRIDGE_H
#define VTDBRIDGE_H

#include "iSimulationData.h"
#include "../mapper/Mapper.h"

class VTDBridge : public iSimulationData
{
public:
	VTDBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int writeToInternalState() override;
	int readFromInternalState() override;
	int doStep(double stepSize = 1) override;

};

#endif // !VTDBRIDGE_H