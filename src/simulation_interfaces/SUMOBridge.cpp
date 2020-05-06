#include "simulation_interfaces/SUMOBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int SUMOBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int SUMOBridge::connect(std::string) {
	return 0;
}

int SUMOBridge::disconnect() {
	return 0;
}

std::vector<int> SUMOBridge::updateInteger() {
	return std::vector<int>();
}

std::vector<float> SUMOBridge::updateFloat() {
	return std::vector<float>();
}

std::vector<double> SUMOBridge::updateDouble() {
	return std::vector<double>();
}

std::vector<bool> SUMOBridge::updateBool() {
	return std::vector<bool>();
}

std::vector<std::string> SUMOBridge::updateString() {
	return std::vector<std::string>();
}

int SUMOBridge::doStep() {
	return 0;
}