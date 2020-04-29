#include "simulation_interfaces/ROSBridge.h"
#include "simulation_interfaces/iSimulationData.h"
#include <string>

int ROSBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int ROSBridge::connect(std::string) {
	return 0;
}

int ROSBridge::disconnect() {
	return 0;
}

std::vector<int> ROSBridge::getInteger() {
	return std::vector<int>();
}

std::vector<float> ROSBridge::getFloat() {
	return std::vector<float>();
}

std::vector<double> ROSBridge::getDouble() {
	return std::vector<double>();
}

std::vector<bool> ROSBridge::getBool() {
	return std::vector<bool>();
}

std::vector<std::string> ROSBridge::getString() {
	return std::vector<std::string>();
}

void ROSBridge::setInteger(std::vector<int>) {}
void ROSBridge::setFloat(std::vector<float>) {}
void ROSBridge::setDouble(std::vector<double>) {}
void ROSBridge::setBool(std::vector<bool>) {}
void ROSBridge::setString(std::vector<std::string>) {}