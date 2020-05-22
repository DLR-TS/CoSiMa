#include <string>
#include "simulation_interfaces/VTDBridge.h"

int VTDBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int VTDBridge::connect(std::string) {
	return 0;
}

int VTDBridge::disconnect() {
	return 0;
}

int VTDBridge::readOutputs() {
	return 0;
}

int VTDBridge::doStep(double stepSize) {
	return 0;
}

void VTDBridge::mapTo(values_t value, std::string interfaceName, eDataType type) {

}