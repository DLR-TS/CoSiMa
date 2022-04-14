#include "SimulationInterfaceFactory.h"

std::shared_ptr<iSimulationData> SimulationInterfaceFactory::makeInterface(eSimulatorName simulatorname, bool verbose) {
	std::shared_ptr<iSimulationData> newInterface = createInterface(simulatorname, verbose);
	//connect mapper with its interface
	if (newInterface != nullptr) {
		newInterface->getMapper()->setOwner(newInterface);
	}
	return newInterface;
}

std::unique_ptr<iSimulationData> SimulationInterfaceFactory::createInterface(eSimulatorName simulatorname, bool verbose) {
	switch (simulatorname) {
	case FMI:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new FMIBridge(std::shared_ptr<Mapper>((Mapper*)new FMIMapper()), verbose)));
	case DEFAULT:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new DefaultBridge(std::shared_ptr<Mapper>((Mapper*)new DefaultMapper()), verbose)));
	case OSI:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new OSIBridge(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()), verbose)));
	case OSMP:
		return std::unique_ptr<iSimulationData>((iSimulationData*)(new OSMPInterface(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()), verbose)));
	case SIMULATORNAME_ERROR:
		std::cout << "Try to create a simulatorinterface which is not defined." << std::endl;
	}
	return nullptr;
}
