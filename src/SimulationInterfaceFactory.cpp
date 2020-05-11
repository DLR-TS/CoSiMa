#include "SimulationInterfaceFactory.h"

//	VTDBridge(Mapper* mapper) : iSimulationData(std::shared_ptr<Mapper>(mapper)) {};

iSimulationData* SimulationInterfaceFactory::makeInterface(SupportedInterfaces simulator) {
	switch (simulator) {
	case VTD:
		return (iSimulationData*)(new VTDBridge(std::shared_ptr<Mapper>((Mapper*)new VTDMapper())));
	case FMI:
		return (iSimulationData*)(new FMIBridge(std::shared_ptr<Mapper>((Mapper*)new FMIMapper())));
	case SUMO:
		return (iSimulationData*)(new SUMOBridge(std::shared_ptr<Mapper>((Mapper*)new SUMOMapper())));
	case OSI:
		//return std::make_unique<iSimulationData>(std::shared_ptr<Mapper>(OSIBridge((Mapper*)new OSIMapper())));
		break;
	case UNREAL:
		return (iSimulationData*)(new UnrealBridge(std::shared_ptr<Mapper>((Mapper*)new UnrealMapper())));
	case ROS:
		return (iSimulationData*)(new ROSBridge(std::shared_ptr<Mapper>((Mapper*)new ROSMapper())));
	}
	return nullptr;
}