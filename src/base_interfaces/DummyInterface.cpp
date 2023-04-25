#include "base_interfaces/DummyInterface.h"

void DummyInterface::configure(YAML::detail::iterator_value& node) {
	config = node.as<DummyInterfaceConfig>();
}

int DummyInterface::init(bool verbose) { return 0; }
double DummyInterface::doStep(double stepSize) { return 0; }
double DummyInterface::getStepSize() { return config.deltaSeconds; }
int DummyInterface::disconnect() { return 0; }

int DummyInterface::getIntValue(std::string base_name) { return 0; }
bool DummyInterface::getBoolValue(std::string base_name) { return 0; }
float DummyInterface::getFloatValue(std::string base_name) { return 0; }
double DummyInterface::getDoubleValue(std::string base_name) { return 0; }
std::string DummyInterface::getStringValue(std::string base_name) {
	auto iter = varName2MessageMap.find(base_name);
	if (iter != varName2MessageMap.end()) {
		return iter->second;
	}
	return "";
}

int DummyInterface::setIntValue(std::string base_name, int value) { return 0; }
int DummyInterface::setBoolValue(std::string base_name, bool value) { return 0; }
int DummyInterface::setFloatValue(std::string base_name, float value) { return 0; }
int DummyInterface::setDoubleValue(std::string base_name, double value) { return 0; }
int DummyInterface::setStringValue(std::string base_name, std::string value) {
	varName2MessageMap[base_name] = value;
	return 0; 
}
