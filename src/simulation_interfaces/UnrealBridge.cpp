#include "simulation_interfaces/UnrealBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int UnrealBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int UnrealBridge::connect(std::string) {
	return 0;
}

int UnrealBridge::disconnect() {
	return 0;
}

int UnrealBridge::readOutputs() {
	return 0;
}

int UnrealBridge::doStep() {
	return 0;
}