/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef DEFAULTBRIDGE_H
#define DEFAULTBRIDGE_H

#include "iSimulationData.h"
#include "../mapper/Mapper.h"

class DefaultBridge : public iSimulationData
{
public:
	DefaultBridge(std::shared_ptr<Mapper> mapper, bool debug) : iSimulationData(mapper, debug){};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int writeToInternalState() override;
	int readFromInternalState() override;
	int doStep(double stepSize = 1) override;

};

#endif // !DEFAULTBRIDGE_H
