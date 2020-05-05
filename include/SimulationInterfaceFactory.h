#ifndef SIMULATIONINTERFACEFACTORY_H
#define SIMULATIONINTERFACEFACTORY_H

#include <string>
//simulation interfaces
#include "simulation_interfaces/iSimulationData.h"
#include "simulation_interfaces/DCPBridge.h"
#include "simulation_interfaces/FMIBridge.h"
#include "simulation_interfaces/ROSBridge.h"
#include "simulation_interfaces/SUMOBridge.h"
#include "simulation_interfaces/UnrealBridge.h"
#include "simulation_interfaces/VTDBridge.h"
//Mapper
#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "mapper/OSIMapper.h"
#include "mapper/ROSMapper.h"
#include "mapper/SUMOMapper.h"
#include "mapper/UnrealMapper.h"
#include "mapper/VTDMapper.h"

class SimulationInterfaceFactory {
public:
	static iSimulationData* makeInterface(std::string name);

};

#endif // !SIMULATIONINTERFACEFACTORY_H
