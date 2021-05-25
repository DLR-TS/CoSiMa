#include "SimulationInterfaceFactory.h"

std::shared_ptr<iSimulationData> SimulationInterfaceFactory::makeInterface(eSimulatorName simulatorname) {
	std::shared_ptr<iSimulationData> newInterface = createInterface(simulatorname);
	//connect mapper with its interface
	if (newInterface != nullptr) {
		newInterface->getMapper()->setOwner(newInterface);
	}
	return newInterface;
}

std::shared_ptr<iSimulationData> SimulationInterfaceFactory::createInterface(eSimulatorName simulatorname) {
	switch (simulatorname) {
	case VTD:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new VTDBridge(std::shared_ptr<Mapper>((Mapper*)new VTDMapper()))));
	case FMI:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new FMIBridge(std::shared_ptr<Mapper>((Mapper*)new FMIMapper()))));
	case DEFAULT:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new DefaultBridge(std::shared_ptr<Mapper>((Mapper*)new DefaultMapper()))));
	case OSI:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new OSIBridge(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()))));
	case OSMP:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new OSMPInterface(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()))));
	case UNREAL:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new UnrealBridge(std::shared_ptr<Mapper>((Mapper*)new UnrealMapper()))));
	case ROS:
		return std::shared_ptr<iSimulationData>((iSimulationData*)(new ROSBridge(std::shared_ptr<Mapper>((Mapper*)new ROSMapper()))));
	case SIMULATORNAME_ERROR:
		std::cout << "Try to create a simulatorinterface which is not defined." << std::endl;
	}
	return nullptr;
}