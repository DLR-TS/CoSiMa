#include <string>
#include "simulation_interfaces/SUMOBridge.h"

int SUMOBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int SUMOBridge::connect(std::string) {
	return 0;
}

int SUMOBridge::disconnect() {
	return 0;
}

int SUMOBridge::readOutputs() {
	return 0;
}

int SUMOBridge::doStep(double stepSize) {
	return 0;
}

void SUMOBridge::mapTo(values_t value, std::string interfaceName, eDataType type) {

}