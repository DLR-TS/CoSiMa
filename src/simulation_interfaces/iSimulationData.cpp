#include "simulation_interfaces/iSimulationData.h"

int iSimulationData::update() {
	state->integers = updateInteger();
	state->floats = updateFloat();
	state->doubles = updateDouble();
	state->bools = updateBool();
	state->strings = updateString();
	return 0;
}

const std::shared_ptr<internalState> iSimulationData::getInternalState() {
	return state;
}

int iSimulationData::mapToOtherInterfaces() {
	return mapper->extractInternalState(state);
}

std::shared_ptr<Mapper> iSimulationData::getMapper() {
	return mapper;
}