#include "FMIBridge.h"
#include "iSimulationData.h"
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

std::vector<int> FMIBridge::getInteger() {
	return std::vector<int>();
}

std::vector<float> FMIBridge::getFloat() {
	return std::vector<float>();
}

std::vector<double> FMIBridge::getDouble() {
	return std::vector<double>();
}

std::vector<bool> FMIBridge::getBool() {
	return std::vector<bool>();
}

std::vector<std::string> FMIBridge::getString() {
	return std::vector<std::string>();
}

void FMIBridge::setInteger(std::vector<int>) {}
void FMIBridge::setFloat(std::vector<float>) {}
void FMIBridge::setDouble(std::vector<double>) {}
void FMIBridge::setBool(std::vector<bool>) {}
void FMIBridge::setString(std::vector<std::string>) {}