#include <string>
#include "simulation_interfaces/UnrealBridge.h"

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

int UnrealBridge::doStep(double stepSize) {
	return 0;
}

void UnrealBridge::mapTo(values_t value, std::string interfaceName, eDataType type) {

}