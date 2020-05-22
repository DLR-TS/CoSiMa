#include <string>
#include "simulation_interfaces/FMIBridge.h"

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

int FMIBridge::doStep(double stepSize) {
	return 0;
}

void FMIBridge::mapTo(values_t value, std::string interfaceName, eDataType type) {

}
