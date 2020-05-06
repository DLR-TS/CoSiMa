#include "SimulationInterfaceFactory.h"

iSimulationData* SimulationInterfaceFactory::makeInterface(SupportedInterfaces simulator) {
	switch (simulator) {
	case VTD:
		return (iSimulationData*)(new VTDBridge((Mapper*)new VTDMapper()));
	case FMI:
		return (iSimulationData*)(new FMIBridge((Mapper*)new FMIMapper()));
	case SUMO:
		return (iSimulationData*)(new SUMOBridge((Mapper*)new SUMOMapper()));
	case OSI:
		//return std::make_unique<iSimulationData>(OSIBridge((Mapper*)new OSIMapper()));
		break;
	case UNREAL:
		return (iSimulationData*)(new UnrealBridge((Mapper*)new UnrealMapper()));
	case ROS:
		return (iSimulationData*)(new ROSBridge((Mapper*)new ROSMapper()));
	}
	return nullptr;
}