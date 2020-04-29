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

std::vector<int> DCPBridge::getInteger() {
	return std::vector<int>();
}

std::vector<float> DCPBridge::getFloat() {
	return std::vector<float>();
}

std::vector<double> DCPBridge::getDouble() {
	return std::vector<double>();
}

std::vector<bool> DCPBridge::getBool() {
	return std::vector<bool>();
}

std::vector<std::string> DCPBridge::getString() {
	return std::vector<std::string>();
}

void DCPBridge::setInteger(std::vector<int>) {}
void DCPBridge::setFloat(std::vector<float>) {}
void DCPBridge::setDouble(std::vector<double>) {}
void DCPBridge::setBool(std::vector<bool>) {}
void DCPBridge::setString(std::vector<std::string>) {}