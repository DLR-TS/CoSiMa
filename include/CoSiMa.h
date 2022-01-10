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
#include "base_interfaces/CARLAInterface.h"
#include "simulation_interfaces/iSimulationData.h"
#include "configreader/YAMLConfigReader.h"
#include "SimulationInterfaceFactory.h"

struct cmdParameter {
    bool log = false;
    bool debug = false;
    bool logOSI = false;
    std::string logPath;
};

void simulationLoop(std::vector<std::shared_ptr<iSimulationData>> &simulationInterfaces,
	std::shared_ptr <BaseSystemInterface> &baseSystem, const cmdParameter& debug);

#endif // !COSIMA_H
