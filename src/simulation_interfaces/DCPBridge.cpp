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

std::vector<int> DCPBridge::updateInteger() {
	return std::vector<int>();
}

std::vector<float> DCPBridge::updateFloat() {
	return std::vector<float>();
}

std::vector<double> DCPBridge::updateDouble() {
	return std::vector<double>();
}

std::vector<bool> DCPBridge::updateBool() {
	return std::vector<bool>();
}

std::vector<std::string> DCPBridge::updateString() {
	return std::vector<std::string>();
}
