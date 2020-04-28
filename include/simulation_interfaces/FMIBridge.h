#ifndef FMIBRIDGE_H
#define FMIBRIDGE_H

#include "iSimulationData.h"
#include "../Mapper/Mapper.h"

class FMIBridge : iSimulationData
{
	FMIBridge(Mapper* mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;
	int doStep() override;

	std::vector<int> getInteger() override;
	std::vector<float> getFloat() override;
	std::vector<double> getDouble() override;
	std::vector<bool> getBool() override;
	std::vector<std::string> getString() override;

	void setInteger(std::vector<int>) override;
	void setFloat(std::vector<float>) override;
	void setDouble(std::vector<double>) override;
	void setBool(std::vector<bool>) override;
	void setString(std::vector<std::string>) override;
};

#endif // !FMIBRIDGE_H