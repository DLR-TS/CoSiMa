/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef COSIMA_H
#define COSIMA_H

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "base_interfaces/BaseSystemInterface.h"
#include "base_interfaces/DummyInterface.h"
#include "base_interfaces/CARLAInterface.h"
#include "simulation_interfaces/iSimulationData.h"
#include "reader/YAMLConfigReader.h"
#include "SimulationInterfaceFactory.h"

struct cmdParameter {
	bool verbose = false;
};

void prepareSimulationStep(std::shared_ptr<iSimulationData> simInterface, std::shared_ptr<BaseSystemInterface> baseSystem);
void doSimulationStep(std::shared_ptr<iSimulationData> simInterface, double stepsize);
void postSimulationStep(std::shared_ptr<iSimulationData> simInterface, std::shared_ptr<BaseSystemInterface> baseSystem);

class Cosima
{
private:
	const cmdParameter runtimeParameter;

	std::shared_ptr<BaseSystemInterface> baseSystem;
	/**
	* Vector that holds every simulation interface.
	*/
	std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces;

public:
	Cosima(cmdParameter cmdParameter) : runtimeParameter{ cmdParameter } {}

	void loadConfiguration(std::string& configurationPath);
	void initInterfaces();
	void sensorViewConfiguration();
	void simulationLoop();

	void disconnect();
};

#endif // !COSIMA_H
