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

struct CmdParameter {
	bool verbose = false;
	bool parallel = false;
	bool sensorviewconfiguration = false;
	std::string configurationPath;
	std::string configurationName = "config.yml";
};

CmdParameter parseRuntimeParameter(int argc, char *argv[]);

class Cosima {
private:

	CmdParameter runtimeParameter;
	SimulationSetup setup;

public:
	void setRuntimeParameter(CmdParameter& runtimeParameter);
	void loadConfiguration();
	void initInterfaces();
	void sensorViewConfiguration();
	void simulationLoop();
	void simulationLoopParallel();

	void prepareSimulationStep(std::shared_ptr<SimulatorInterface> simInterface);
	void doSimulationStep(std::shared_ptr<SimulatorInterface> simInterface);
	void postSimulationStep(std::shared_ptr<SimulatorInterface> simInterface);
	void stopSimulation(std::shared_ptr<SimulatorInterface> simInterface);

	void disconnect();
};

#endif // !COSIMA_H
