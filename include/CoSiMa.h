/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef COSIMA_H
#define COSIMA_H

#cmakedefine WITH_CARLA

#include <iostream>
#include <vector>
#include <string>
#include "base_interfaces/BaseSystemInterface.h"
//#ifdef WITH_CARLA
//#include "base_interfaces/CARLAInterface.h"
//#endif
#include "simulation_interfaces/iSimulationData.h"
#include "configreader/YAMLConfigReader.h"
#include "SimulationInterfaceFactory.h"


void simulationLoop(std::vector<std::shared_ptr<iSimulationData>> &simulationInterfaces, std::shared_ptr <BaseSystemInterface> &baseSystem, bool& debug);

#endif // !COSIMA_H
