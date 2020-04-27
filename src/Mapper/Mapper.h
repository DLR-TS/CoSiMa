#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <vector>
#include <variant>
#include "../SimulationInterfaces/iSimulationData.h"
#include "../SimulationInterfaces/internalState.h"

typedef std::variant<int, float, double, bool, std::string> values_t;

class MapperConfig {
public:
	std::vector<std::string> intInputVector;
	std::vector<std::string> intOutputVector;
	std::vector<std::string> floatInputVector;
	std::vector<std::string> floatOutputVector;
	std::vector<std::string> doubleInputVector;
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
	virtual void mapTo(values_t value, std::string name, std::string type) = 0;
};

#endif // !MAPPER_H
