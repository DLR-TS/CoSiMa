#ifndef COSIMA_H
#define COSIMA_H

#include <iostream>
#include <vector>
#include <string>
#include "simulation_interfaces/iSimulationData.h"
#include "configreader/YAMLConfigReader.h"

/**
* Vector that holds every simulation interface.
*/
static std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces;

#endif // !COSIMA_H
