#include "simulation_interfaces/DCPBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int DCPBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int DCPBridge::connect(std::string) {
	return 0;
}

int DCPBridge::disconnect() {
	return 0;
}

int DCPBridge::readOutputs() {
	return 0;
}

int DCPBridge::doStep() {
	return 0;
}

void DCPBridge::mapTo(values_t value, std::string interfaceName, eDataType type) {

}