#include <string>
#include "simulation_interfaces/ROSBridge.h"

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

int ROSBridge::doStep(double stepSize) {
	return 0;
}

void ROSBridge::mapTo(values_t value, std::string interfaceName, eDataType type) {

}