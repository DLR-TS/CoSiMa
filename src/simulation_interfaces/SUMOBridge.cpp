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

std::vector<int> SUMOBridge::getInteger() {
	return std::vector<int>();
}

std::vector<float> SUMOBridge::getFloat() {
	return std::vector<float>();
}

std::vector<double> SUMOBridge::getDouble() {
	return std::vector<double>();
}

std::vector<bool> SUMOBridge::getBool() {
	return std::vector<bool>();
}

std::vector<std::string> SUMOBridge::getString() {
	return std::vector<std::string>();
}

void SUMOBridge::setInteger(std::vector<int>) {}
void SUMOBridge::setFloat(std::vector<float>) {}
void SUMOBridge::setDouble(std::vector<double>) {}
void SUMOBridge::setBool(std::vector<bool>) {}
void SUMOBridge::setString(std::vector<std::string>) {}