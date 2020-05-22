#include "catch2/catch.hpp"

#include "simulation_interfaces/iSimulationData.h"
#include "mapper/Mapper.h" 

class MockInterfaceSimulator : public iSimulationData {
public:
	MockInterfaceSimulator(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) {
		return 0;
	}

	int connect(std::string) {
		return 0;
	}

	int disconnect() {
		return 0;
	}

	int readOutputs() {
		return 0;
	}

	int doStep(double stepSize = 1) {
		return 0;
	}

	void mapTo(values_t value, std::string interfaceName, eDataType type) {
	
	}

	std::shared_ptr<internalState> getInternalState() {
		return getInternalState();
	}
};
