#include "simulation_interfaces/UnrealBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int UnrealBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int UnrealBridge::connect(std::string) {
	return 0;
}

int UnrealBridge::disconnect() {
	return 0;
}

std::vector<int> UnrealBridge::updateInteger() {
	return std::vector<int>();
}

std::vector<float> UnrealBridge::updateFloat() {
	return std::vector<float>();
}

std::vector<double> UnrealBridge::updateDouble() {
	return std::vector<double>();
}

std::vector<bool> UnrealBridge::updateBool() {
	return std::vector<bool>();
}

std::vector<std::string> UnrealBridge::updateString() {
	return std::vector<std::string>();
}

int UnrealBridge::doStep() {
	return 0;
}