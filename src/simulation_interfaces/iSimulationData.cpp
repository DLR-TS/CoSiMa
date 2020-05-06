#include "simulation_interfaces/iSimulationData.h"

int iSimulationData::update() {
	state.integers = updateInteger();
	state.floats = updateFloat();
	state.doubles = updateDouble();
	state.bools = updateBool();
	state.strings = updateString();
	return 0;
}

const internalState iSimulationData::getInternalState() {
	return state;
}

int iSimulationData::mapToOtherInterfaces() {
	return mapper->extractInternalState(&state);
}

Mapper* iSimulationData::getMapper() {
	return mapper;
}