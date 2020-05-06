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

std::vector<int> ROSBridge::updateInteger() {
	return std::vector<int>();
}

std::vector<float> ROSBridge::updateFloat() {
	return std::vector<float>();
}

std::vector<double> ROSBridge::updateDouble() {
	return std::vector<double>();
}

std::vector<bool> ROSBridge::updateBool() {
	return std::vector<bool>();
}

std::vector<std::string> ROSBridge::updateString() {
	return std::vector<std::string>();
}

int ROSBridge::doStep(){
	return 0;
}