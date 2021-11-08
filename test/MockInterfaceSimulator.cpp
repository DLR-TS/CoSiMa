/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "simulation_interfaces/iSimulationData.h"
#include "mapper/Mapper.h" 

class MockInterfaceSimulator : public iSimulationData {
public:
	MockInterfaceSimulator(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper, false) {};

	int init(float starttime) {
		return 0;
	}

	int connect(std::string) {
		return 0;
	}

	int disconnect() {
		return 0;
	}

	int writeToInternalState() {
		return 0;
	}

	int doStep(double stepSize = 1) {
		return 0;
	}

	int readFromInternalState() {
		return 0;
	}

	std::shared_ptr<internalState> getInternalState() {
		return getInternalState();
	}
};
