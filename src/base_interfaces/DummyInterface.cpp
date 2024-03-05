#include "base_interfaces/DummyInterface.h"

void DummyInterface::configure(YAML::Node& node) {
	config = node.as<DummyInterfaceConfig>();
}

int DummyInterface::init(bool verbose) { return 0; }
double DummyInterface::doStep(double stepSize) { return 0; }
double DummyInterface::getStepSize() { return config.deltaSeconds; }
void DummyInterface::setStepSize(double stepSize) { config.deltaSeconds = stepSize; }
int DummyInterface::disconnect() { return 0; }

std::string DummyInterface::getOSIMessage(const std::string& base_name) {
	auto iter = varName2MessageMap.find(base_name);
	if (iter != varName2MessageMap.end()) {
		return iter->second;
	}
	return "";
}

int DummyInterface::setOSIMessage(const std::string& base_name, const std::string& value) {
	varName2MessageMap[base_name] = value;
	return 0; 
}
