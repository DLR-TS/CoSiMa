#include "SimulationInterfaceFactory.h"

iSimulationData* SimulationInterfaceFactory::makeInterface(std::string name) {
	if (name == "VTD" || name == "vtd") {
		return (iSimulationData*)new VTDBridge((Mapper*)new VTDMapper());
	}
	else if (name == "FMI" || name == "fmi") {
		return (iSimulationData*)new FMIBridge((Mapper*)new FMIMapper());
	}
	else if (name == "SUMO" || name == "sumo") {
		return (iSimulationData*)new SUMOBridge((Mapper*)new SUMOMapper());
	}
	else if (name == "OSI" || name == "osi") {
		//return (iSimulationData*)new OSIBridge((Mapper*)new OSIMapper());
	}
	else if (name == "UE" || name == "UnrealEngine") {
		return (iSimulationData*)new UnrealBridge((Mapper*)new UnrealMapper());
	}
	else if (name == "ROS" || name == "ros") {
		return (iSimulationData*)new ROSBridge((Mapper*)new ROSMapper());
	}
	return nullptr;
}