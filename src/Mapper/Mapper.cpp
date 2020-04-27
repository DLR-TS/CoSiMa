#include "Mapper.h"
#include <string>
#include "../CoSiMa.h"

//output part
int Mapper::extractInternalState(internalState* state)
{
	for (auto simulationInterface : simulationInterfaces)
	{
		Mapper* inputMapper = simulationInterface->getMapper();
		inputMapper->filterInput(this, state);
	}
	return 0;
}

//called from output part of Mapper
void Mapper::filterInput(Mapper* outputMapper, internalState* foreignState) {
	//integer
	for (unsigned int index = 0; index < outputMapper->config.intOutputVector.size(); index++) {
		for (std::string name : config.intInputVector) {
			if (name.compare(config.intOutputVector.at(index))) {
				//each Mapper implementation handles this
				mapTo(foreignState->integers.at(index), name, "integer");
			}
		}
	}
	//float
	for (unsigned int index = 0; index < outputMapper->config.floatOutputVector.size(); index++) {
		for (std::string name : config.floatInputVector) {
			if (name.compare(config.floatOutputVector.at(index))) {
				//each Mapper implementation handles this
				mapTo(foreignState->floats.at(index), name, "float");
			}
		}
	}
	//double
	for (unsigned int index = 0; index < outputMapper->config.doubleOutputVector.size(); index++) {
		for (std::string name : config.doubleInputVector) {
			if (name.compare(config.doubleOutputVector.at(index))) {
				//each Mapper implementation handles this
				mapTo(foreignState->doubles.at(index), name, "double");
			}
		}
	}
	//bool
	for (unsigned int index = 0; index < outputMapper->config.boolOutputVector.size(); index++) {
		for (std::string name : config.boolInputVector) {
			if (name.compare(config.boolOutputVector.at(index))) {
				//each Mapper implementation handles this
				mapTo(foreignState->bools.at(index), name, "bool");
			}
		}
	}
	//string
	for (unsigned int index = 0; index < outputMapper->config.stringOutputVector.size(); index++) {
		for (std::string name : config.stringInputVector) {
			if (name.compare(config.stringOutputVector.at(index))) {
				//each Mapper implementation handles this
				mapTo(foreignState->strings.at(index), name, "string");
			}
		}
	}
}

int Mapper::readConfiguration(std::string) {
	//some reading of triples
	//(interface_name, base_name, type)

	//result
	//filled input and output vectors
	return 0;
}

/*
TDominion mapToDominion(std::string variablename)
{
	if (variablename == "MoSAIC.EgoVehicle.Kinematics.Position" && variablename is input) return _input.MoSAIC..... else _output.MoSAIC....

		// alternative

		switch (variablename)
		{
		case variablename.equals("MoSAIC.EgoVehicle.Kinematics.Position"):
			if (input) return _input....;
			else return _output....;
			break;
		}

}

typedef OSI  std::variant<osi3::GroundTruth, osi3::SensorData>


// FMI Mapper converts (length, StartPointer, EndPointer) triple into input stream
OSI mapToFMI(std::string variablename, fstream input(argv[1], ios::in | ios::binary))
{
	if (variablename.equals("osi3::GroundTruth") return osi3::GroundTruth.ParseFromIstream(&input)

		// alternative

		switch (variablename)
		{
		case variablename.equals("MoSAIC.EgoVehicle.Kinematics.Position"):
			if (input) return _input....;
			else return _output....;
				break;
		}

}
*/