#include "simulation_interfaces/iSimulationData.h"

const std::shared_ptr<internalState> iSimulationData::getInternalState() {
	return state;
}

int iSimulationData::mapInput(std::shared_ptr<BaseSystemInterface> baseInterface) {
	return mapper->searchInput(baseInterface);
}

int iSimulationData::writeTo(std::shared_ptr<BaseSystemInterface> baseInterface) {
	return mapper->writeOutput(baseInterface);
}

std::shared_ptr<Mapper> iSimulationData::getMapper() {
	return mapper;
}