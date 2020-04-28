#ifndef SIMULATIONINTERFACEFACTORY_H
#define SIMULATIONINTERFACEFACTORY_H

#include <string>
#include "simulation_interfaces/iSimulationData.h"

class SimulationInterfaceFactory {
public:
	static iSimulationData* makeInterface(std::string name);

};

#endif // !SIMULATIONINTERFACEFACTORY_H
