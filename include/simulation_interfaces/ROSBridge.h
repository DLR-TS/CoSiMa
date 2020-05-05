#ifndef ROSBRIDGE_H
#define ROSBRIDGE_H

#include "simulation_interfaces/iSimulationData.h"
#include "../Mapper/Mapper.h"

class ROSBridge : iSimulationData
{
public:
	ROSBridge(Mapper* mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

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
	int doStep() override;
};

#endif // !ROSBRIDGE_H