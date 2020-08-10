#include "base_interfaces\CARLAInterface.h"

int CARLAInterface::readConfiguration(baseConfigVariants_t variant) {
	CARLAInterfaceConfig* config = std::get_if<CARLAInterfaceConfig>(&variant);
	if (nullptr == config) {
		std::cerr << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	this->config.reset(std::move(config));

	return 0;
}

int CARLAInterface::initialise() {
	return 0;
}

int CARLAInterface::getIntValue(std::string base_name) {
	return 0;
};

bool CARLAInterface::getBoolValue(std::string base_name) {
	return true;
};

float CARLAInterface::getFloatValue(std::string base_name) {
	return 0.0;
};

double CARLAInterface::getDoubleValue(std::string base_name) {
	return 0.0;
};

std::string CARLAInterface::getStringValue(std::string base_name) {
	return "";
};

int CARLAInterface::setIntValue(std::string base_name, int value) {
	return 0;
};

int CARLAInterface::setBoolValue(std::string base_name, bool value) {
	return 0;
};

int CARLAInterface::setFloatValue(std::string base_name, float value) {
	return 0;
};

int CARLAInterface::setDoubleValue(std::string base_name, double value) {
	return 0;
};

int CARLAInterface::setStringValue(std::string base_name, std::string value) {
	return 0;
};
