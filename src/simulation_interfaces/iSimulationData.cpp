#include "simulation_interfaces/iSimulationData.h"

void iSimulationData::mapToInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface) {
	mapper->searchInput(baseInterface);
}

void iSimulationData::mapFromInterfaceSystem(std::shared_ptr<BaseSystemInterface> baseInterface) {
	mapper->writeOutput(baseInterface);
}

std::shared_ptr<Mapper> iSimulationData::getMapper() {
	return mapper;
}

int iSimulationData::readConfiguration(configVariants_t configVariants) {
	std::cout << "Calling the read configuration method, which is not implemented by selected interface: " <<
	std::get_if<InterfaceYAMLConfig>(&configVariants)->simulator << std::endl;
	throw 1;
	return 0;
}
