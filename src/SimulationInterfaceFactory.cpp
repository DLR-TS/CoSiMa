#include "SimulationInterfaceFactory.h"

iSimulationData* SimulationInterfaceFactory::makeInterface(std::string simulator) {
	if (simulator == "VTD" || simulator == "vtd") {
		return (iSimulationData*)new VTDBridge((Mapper*)new VTDMapper());
	}
	else if (simulator == "FMI" || simulator == "fmi") {
		return (iSimulationData*)new FMIBridge((Mapper*)new FMIMapper());
	}
	else if (simulator == "SUMO" || simulator == "sumo") {
		return (iSimulationData*)new SUMOBridge((Mapper*)new SUMOMapper());
	}
	else if (simulator == "OSI" || simulator == "osi") {
		//return (iSimulationData*)new OSIBridge((Mapper*)new OSIMapper());
	}
	else if (simulator == "UE" || simulator == "UnrealEngine") {
		return (iSimulationData*)new UnrealBridge((Mapper*)new UnrealMapper());
	}
	else if (simulator == "ROS" || simulator == "ros") {
		return (iSimulationData*)new ROSBridge((Mapper*)new ROSMapper());
	}
	return nullptr;
}