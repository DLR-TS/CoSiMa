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

std::vector<int> UnrealBridge::getInteger() {
	return std::vector<int>();
}

std::vector<float> UnrealBridge::getFloat() {
	return std::vector<float>();
}

std::vector<double> UnrealBridge::getDouble() {
	return std::vector<double>();
}

std::vector<bool> UnrealBridge::getBool() {
	return std::vector<bool>();
}

std::vector<std::string> UnrealBridge::getString() {
	return std::vector<std::string>();
}

void UnrealBridge::setInteger(std::vector<int>) {}
void UnrealBridge::setFloat(std::vector<float>) {}
void UnrealBridge::setDouble(std::vector<double>) {}
void UnrealBridge::setBool(std::vector<bool>) {}
void UnrealBridge::setString(std::vector<std::string>) {}