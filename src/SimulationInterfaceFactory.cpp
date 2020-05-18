﻿#include "SimulationInterfaceFactory.h"

std::shared_ptr<iSimulationData> SimulationInterfaceFactory::makeInterface(eSimulatorName simulatorname) {
	std::shared_ptr<iSimulationData> newInterface = createInterface(simulatorname);
	//connect mapper with its interface
	newInterface->getMapper()->setOwner(newInterface);
	return newInterface;
}

std::shared_ptr<iSimulationData> SimulationInterfaceFactory::createInterface(eSimulatorName simulatorname) {
	switch (simulatorname) {
	case VTD:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new VTDBridge(std::shared_ptr<Mapper>((Mapper*)new VTDMapper()))));
	case FMI:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new FMIBridge(std::shared_ptr<Mapper>((Mapper*)new FMIMapper()))));
	case SUMO:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new SUMOBridge(std::shared_ptr<Mapper>((Mapper*)new SUMOMapper()))));
	case OSI:
		//return std::shared_ptr<iSimulationData>((iSimulationData*)(new OSIBridge(std::shard_ptr<Mapper>((Mapper*)new OSIMapper()))));
		break;
	case UNREAL:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new UnrealBridge(std::shared_ptr<Mapper>((Mapper*)new UnrealMapper()))));
	case ROS:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new ROSBridge(std::shared_ptr<Mapper>((Mapper*)new ROSMapper()))));
	}
	return nullptr;
}