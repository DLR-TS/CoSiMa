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

std::vector<int> VTDBridge::getInteger() {
	return std::vector<int>();
}

std::vector<float> VTDBridge::getFloat() {
	return std::vector<float>();
}

std::vector<double> VTDBridge::getDouble() {
	return std::vector<double>();
}

std::vector<bool> VTDBridge::getBool() {
	return std::vector<bool>();
}

std::vector<std::string> VTDBridge::getString() {
	return std::vector<std::string>();
}

void VTDBridge::setInteger(std::vector<int>) {}
void VTDBridge::setFloat(std::vector<float>) {}
void VTDBridge::setDouble(std::vector<double>) {}
void VTDBridge::setBool(std::vector<bool>) {}
void VTDBridge::setString(std::vector<std::string>) {}