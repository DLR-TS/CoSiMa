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

	virtual void configure(const YAML::Node& node) override {
	}

	virtual bool isAutostart(uint16_t& port) override {
		return false;
	};

	virtual int init(bool verbose) override {
		return 0;
	}

	virtual double doStep(double stepSize = 1) override {
		return 0;
	}

	virtual int disconnect() override {
		return 0;
	}

	virtual double getStepSize() override {
		return 0.03;
	}
	virtual int setOSIMessage(const std::string& base_name, const std::string& value) override {
		requestedVariables.push_back(base_name);
		return 0;
	}

	virtual void setStepSize(double stepSize) override {}

	virtual std::string getOSIMessage(const std::string& base_name) override {
		requestedVariables.push_back(base_name);
		return "";
	};
};
