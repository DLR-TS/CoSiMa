#include "simulation_interfaces/iSimulationData.h"

void SimulatorInterface::mapToInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface) {
	mapper->searchInput(baseInterface);
}

void SimulatorInterface::mapFromInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface) {
	mapper->writeOutput(baseInterface);
}

void SimulatorInterface::configureMapperOwner() {
	mapper->setOwner(shared_from_this());
}

void SimulatorInterface::configure(const YAML::Node& node) {
	std::cout << "Calling the read configuration method, which is not implemented by selected interface: " << std::endl;
}
