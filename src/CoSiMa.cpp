#include "CoSiMa.h"
#include "SimulationInterfaceFactory.h"

int main()
{
	std::cout << "Hello CoSiMa." << std::endl;

	//	FMIBridge test = FMIBridge();


	std::vector<std::string> interfacenames;
	//read parameters

	//choose protocol

	//create objects in SimulationInterfaceFactory
	for (std::string interfacename : interfacenames) {
		simulationInterfaces.push_back(SimulationInterfaceFactory::makeInterface(interfacename));
	}

	//init interfaces
	for (auto simInterface : simulationInterfaces) {
		//simInterface->init();
	}

	//start simulationloop
	while (true) {
		for (auto simInterface : simulationInterfaces) {
			//set data (read from internalstate)
			simInterface->mapToOtherInterfaces();
		}

		for (auto simInterface : simulationInterfaces) {
			//do step
			simInterface->doStep();
		}

		for (auto simInterface : simulationInterfaces) {
			//get data (write it to internalState)
			simInterface->update();
		}

	}
	return 0;
}
