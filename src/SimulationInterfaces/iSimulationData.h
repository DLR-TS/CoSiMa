#ifndef ISIMULATIONDATA_H
#define ISIMULATIONDATA_H

#include <vector>
#include <string>
#include "../Mapper/Mapper.h"
#include "internalState.h"

class Mapper;

class iSimulationData
{
protected:
	iSimulationData(Mapper* mapper) {
		this->mapper = mapper;
	}

	internalState state;
	Mapper* mapper;

public:
	virtual int init(std::string scenario, float starttime, int mode) = 0;
	virtual int connect(std::string) = 0;
	virtual int disconnect() = 0;

	int update();
	int mapToOtherInterfaces();
	virtual int doStep() = 0;
	Mapper* getMapper();

protected:
	//getter: get Data from Simulation Interface
	virtual std::vector<int> getInteger() = 0;
	virtual std::vector<float> getFloat() = 0;
	virtual std::vector<double> getDouble() = 0;
	virtual std::vector<bool> getBool() = 0;
	virtual std::vector<std::string> getString() = 0;
public:
	internalState* getInternalState();

public:
	//setter: set Data to Simulation Interface
	virtual void setInteger(std::vector<int>) = 0;
	virtual void setFloat(std::vector<float>) = 0;
	virtual void setDouble(std::vector<double>) = 0;
	virtual void setBool(std::vector<bool>) = 0;
	virtual void setString(std::vector<std::string>) = 0;
};
#endif // !ISIMULATIONDATA_H