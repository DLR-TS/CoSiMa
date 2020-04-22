#include "CoSiMa.h"

int main()
{
	std::cout << "Hello CoSiMa." << std::endl;

//	FMIBridge test = FMIBridge();

	//read parameters

	//choose protocol

	//create objects in SimulationInterfaceFactory

		//init them
		//simulationInterfaces.back(new created sim interface);

	//start simulationloop
	while (true) {
		for (auto simInterface : simulationInterfaces) {
			//get new data from simulation interface input
			simInterface->update();
			//mapping data to other interfaces
			simInterface->mapToOtherInterfaces();
		}

		for (auto simInterface : simulationInterfaces) {
			//send outputs?
			//simInterface->sendOutput();
		}
	}

	return 0;
}
