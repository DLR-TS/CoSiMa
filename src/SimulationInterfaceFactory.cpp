#include "SimulationInterfaceFactory.h"

std::unique_ptr<iSimulationData> SimulationInterfaceFactory::makeInterface(eSimulatorName simulatorname, bool debug) {
	std::unique_ptr<iSimulationData> newInterface = createInterface(simulatorname, debug);
	//connect mapper with its interface
	if (newInterface != nullptr) {
		newInterface->getMapper()->setOwner(newInterface.get());
	}
	return newInterface;
}

std::unique_ptr<iSimulationData> SimulationInterfaceFactory::createInterface(eSimulatorName simulatorname, bool debug) {
	switch (simulatorname) {
	case FMI:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new FMIBridge(std::shared_ptr<Mapper>((Mapper*)new FMIMapper()), debug)));
	case DEFAULT:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new DefaultBridge(std::shared_ptr<Mapper>((Mapper*)new DefaultMapper()), debug)));
	case OSI:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new OSIBridge(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()), debug)));
	case OSMP:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new OSMPInterface(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()), debug)));
	case SIMULATORNAME_ERROR:
		std::cout << "Try to create a simulatorinterface which is not defined." << std::endl;
	}
	return nullptr;
}
