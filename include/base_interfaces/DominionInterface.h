#ifndef DOMINIONINTERFACE_H
#define DOMINIONINTERFACE_H

#include <string>
#include "base_interfaces/BaseSystemInterface.h"

class DominionInterface : BaseSystemInterface
{
	virtual int getIntValue(int base_name) override;
	virtual bool getBoolValue(int base_name) override;
	virtual float getFloatValue(int base_name) override;
	virtual double getDoubleValue(int base_name) override;
	virtual std::string getStringValue(int base_name) override;
};

#endif // !DOMINIONINTERFACE_H