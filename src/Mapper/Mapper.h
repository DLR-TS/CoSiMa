#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <vector>
#include "../SimulationInterfaces/iSimulationData.h"
#include "../SimulationInterfaces/internalState.h"


class MapperConfig {
public:
	std::vector<std::string> intInputVector;
	std::vector<std::string> intOutputVector;
	std::vector<std::string> floatInputVector;
	std::vector<std::string> floatOutputVector;
	std::vector<std::string> doulbeInputVector;
	std::vector<std::string> doubleOutputVector;
	std::vector<std::string> boolInputVector;
	std::vector<std::string> boolOutputVector;
	std::vector<std::string> stringInputVector;
	std::vector<std::string> stringOutputVector;
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
