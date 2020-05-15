#ifndef BASESYSTEMINTERFACE_H
#define BASESYSTEMINTERFACE_H

#include "mapper/Mapper.h"
#include <string>

class BaseSystemInterface
{
public:
	virtual int getIntValue(int base_name) = 0;
	virtual bool getBoolValue(int base_name) = 0;
	virtual float getFloatValue(int base_name) = 0;
	virtual double getDoubleValue(int base_name) = 0;
	virtual std::string getStringValue(int base_name) = 0;
};

#endif // !BASESYSTEMINTERFACE_H