#include <string>
#include "simulation_interfaces/DefaultBridge.h"

int DefaultBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int DefaultBridge::connect(std::string) {
	return 0;
}

int DefaultBridge::disconnect() {
	return 0;
}

int DefaultBridge::writeToInternalState() {
	return 0;
}

int DefaultBridge::doStep(double stepSize) {
	return 0;
}

int DefaultBridge::readFromInternalState() {
	return 0;
}