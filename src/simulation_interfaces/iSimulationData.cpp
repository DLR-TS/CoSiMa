#include "simulation_interfaces/iSimulationData.h"

int iSimulationData::update() {
	state.integers = getInteger();
	state.floats = getFloat();
	state.doubles = getDouble();
	state.bools = getBool();
	state.strings = getString();
	return 0;
}

internalState* iSimulationData::getInternalState() {
	return &state;
}

int iSimulationData::mapToOtherInterfaces() {
	return mapper->extractInternalState(&state);
}

Mapper* iSimulationData::getMapper() {
	return mapper;
}