#include "simulation_interfaces/FMIBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int FMIBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int FMIBridge::connect(std::string) {
	return 0;
}

int FMIBridge::disconnect() {
	return 0;
}

int FMIBridge::doStep() {
	return 0;
}

std::vector<int> FMIBridge::updateInteger() {
	return std::vector<int>();
}

std::vector<float> FMIBridge::updateFloat() {
	return std::vector<float>();
}

std::vector<double> FMIBridge::updateDouble() {
	return std::vector<double>();
}

std::vector<bool> FMIBridge::updateBool() {
	return std::vector<bool>();
}

std::vector<std::string> FMIBridge::updateString() {
	return std::vector<std::string>();
}
