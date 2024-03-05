/**
@authors German Aerospace Center: Björn Bahn
*/

#ifndef DUMMYINTERFACE_H
#define DUMMYINTERFACE_H

#include "BaseSystemInterface.h"

class DummyInterface : public BaseSystemInterface
{
public:
	virtual void configure(YAML::Node& node) override;
	virtual int init(bool debug) override;
	virtual double doStep(double stepSize = 1) override;
	virtual double getStepSize() override;
	virtual void setStepSize(double stepSize) override;
	virtual int disconnect() override;

	virtual std::string getOSIMessage(const std::string& base_name) override;

	virtual int setOSIMessage(const std::string& base_name, const std::string& value) override;

	DummyInterfaceConfig config;
private:
	std::map<std::string, std::string> varName2MessageMap;
};

#endif // !DUMMYINTERFACE_H
