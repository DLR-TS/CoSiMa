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
	//at the moment just for int 
	for (unsigned int index = 0; index < outputMapper->config.intOutputVector.size(); index++) {
		for (std::string name : config.intInputVector) {
			if (name.compare(config.intOutputVector.at(index))) {
				//each Mapper implementation handles this
				mapTo(index, name/*, type*/);
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