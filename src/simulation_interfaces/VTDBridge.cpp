#include "simulation_interfaces/VTDBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

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

int VTDBridge::doStep() {
	return 0;
}