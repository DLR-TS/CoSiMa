#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <vector>
#include "../SimulationInterfaces/iSimulationData.h"
#include "../SimulationInterfaces/internalState.h"


class MapperConfig {
public:
	std::vector<std::string> inputVector;
	std::vector<std::string> outputVector;
};

class Mapper {

private:
	MapperConfig config;

public:
	int extractInternalState(internalState* pointer);
	int readConfiguration(std::string config);

private:
	void filterInput(Mapper* outputMapper, internalState* state);

protected:
	virtual void mapTo(int index, std::string name) = 0;
};

#endif // !MAPPER_H
