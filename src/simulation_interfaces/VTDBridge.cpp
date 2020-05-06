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

std::vector<int> VTDBridge::updateInteger() {
	return std::vector<int>();
}

std::vector<float> VTDBridge::updateFloat() {
	return std::vector<float>();
}

std::vector<double> VTDBridge::updateDouble() {
	return std::vector<double>();
}

std::vector<bool> VTDBridge::updateBool() {
	return std::vector<bool>();
}

std::vector<std::string> VTDBridge::updateString() {
	return std::vector<std::string>();
}

int VTDBridge::doStep() {
	return 0;
}