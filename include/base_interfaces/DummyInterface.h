/**
@authors German Aerospace Center: Björn Bahn
*/

#ifndef DUMMYINTERFACE_H
#define DUMMYINTERFACE_H

#include "reader/BaseConfigVariants.h"
#include "BaseSystemInterface.h"

class DummyInterface : public BaseSystemInterface
{
public:
	virtual void configure(YAML::detail::iterator_value& node) override;
	virtual int init(bool debug) override;
	virtual double doStep(double stepSize = 1) override;
	virtual double getStepSize() override;
	virtual int disconnect() override;

	virtual int getIntValue(std::string base_name) override;
	virtual bool getBoolValue(std::string base_name) override;
	virtual float getFloatValue(std::string base_name) override;
	virtual double getDoubleValue(std::string base_name) override;
	virtual std::string getStringValue(std::string base_name) override;

	virtual int setIntValue(std::string base_name, int value) override;
	virtual int setBoolValue(std::string base_name, bool value) override;
	virtual int setFloatValue(std::string base_name, float value) override;
	virtual int setDoubleValue(std::string base_name, double value) override;
	virtual int setStringValue(std::string base_name, std::string value) override;

	DummyInterfaceConfig config;
private:
	std::map<std::string, std::string> varName2MessageMap;
};

#endif // !DUMMYINTERFACE_H
