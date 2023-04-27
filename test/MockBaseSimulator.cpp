/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "base_interfaces/BaseSystemInterface.h"

class MockBaseSimulator : public BaseSystemInterface {

public:
	int intvalue;
	float floatvalue;
	double doublevalue;
	bool boolvalue;
	std::string stringvalue = "";

	std::vector<std::string> requestedVariables;

	int getIntValue(std::string base_name) {
		requestedVariables.push_back(base_name);
		return intvalue;
	};

	bool getBoolValue(std::string base_name) {
		requestedVariables.push_back(base_name);
		return boolvalue;
	};

	float getFloatValue(std::string base_name) {
		requestedVariables.push_back(base_name);
		return floatvalue;
	};

	double getDoubleValue(std::string base_name) {
		requestedVariables.push_back(base_name);
		return doublevalue;
	};

	std::string getOSIMessage(std::string base_name) {
		requestedVariables.push_back(base_name);
		return stringvalue;
	};

	int setIntValue(std::string base_name, int value) {
		intvalue = value;
		return 0;
	};

	int setBoolValue(std::string base_name, bool value) {
		boolvalue = value;
		return 0;
	};

	int setFloatValue(std::string base_name, float value) {
		floatvalue = value;
		return 0;
	};

	int setDoubleValue(std::string base_name, double value) {
		doublevalue = value;
		return 0;
	};

	int setOSIMessage(std::string base_name, std::string value) {
		stringvalue = value;
		return 0;
	};

	virtual void configure(YAML::detail::iterator_value& node) override {
	};

	virtual int init(bool verbose) override {
		return 0;
	};

	virtual double doStep(double stepSize = 1) override {
		return 0;
	}

	virtual int disconnect() override {
		return 0;
	};

	virtual double getStepSize() override {
		return 0.03;
	};
};
