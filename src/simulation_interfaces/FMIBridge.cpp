#include "simulation_interfaces/FMIBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int FMIBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int FMIBridge::connect(std::string) {
	return 0;
}

int FMIBridge::disconnect() {
	return 0;
}

int FMIBridge::readOutputs() {
	return 0;
}

int FMIBridge::doStep() {
	return 0;
}
