#include "simulation_interfaces/iSimulationData.h"
#include <memory.h>

void SimulatorInterface::mapToInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface) {
	mapper->searchInput(baseInterface);
}

void SimulatorInterface::mapFromInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface) {
	mapper->writeOutput(baseInterface);
}

void SimulatorInterface::configureMapper() {
	mapper->setOwner(shared_from_this());
}

void SimulatorInterface::configure(YAML::detail::iterator_value& node) {
	std::cout << "Calling the read configuration method, which is not implemented by selected interface: " << std::endl;
}
