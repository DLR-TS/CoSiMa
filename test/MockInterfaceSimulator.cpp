/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "simulation_interfaces/iSimulationData.h"
#include "mapper/Mapper.h" 

class MockInterfaceSimulator : public SimulatorInterface {
public:
	MockInterfaceSimulator(std::shared_ptr<Mapper> mapper) {};

	int init() {

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

	virtual int init(bool verbose, std::string configurationPath) {
		return 0;
	}
};
