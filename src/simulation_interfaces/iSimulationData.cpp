#include "simulation_interfaces/iSimulationData.h"

/*
int iSimulationData::readOutput() {
	state->integers = updateInteger();
	state->floats = updateFloat();
	state->doubles = updateDouble();
	state->bools = updateBool();
	state->strings = updateString();
	return 0;
}*/

const std::shared_ptr<internalState> iSimulationData::getInternalState() {
	return state;
}

int iSimulationData::mapInput(std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces) {
	return mapper->searchInput(simulationInterfaces);
}

std::shared_ptr<Mapper> iSimulationData::getMapper() {
	return mapper;
}