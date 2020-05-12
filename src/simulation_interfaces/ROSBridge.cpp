#include "simulation_interfaces/ROSBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int ROSBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int ROSBridge::connect(std::string) {
	return 0;
}

int ROSBridge::disconnect() {
	return 0;
}

int ROSBridge::readOutputs() {
	return 0;
}

int ROSBridge::doStep(){
	return 0;
}