#include "CoSiMa.h"
#include "SimulationInterfaceFactory.h"

int main()
{
	std::cout << "Hello CoSiMa." << std::endl;



	//read config
	YAMLConfigReader reader = YAMLConfigReader("D:/config.yaml");//path shall be set as parameter of CoSiMa
	std::vector<std::string>* interfacenames = reader.getInterfaceNames();
	std::cout << "Hello CoSiMa2." << std::endl;
	//choose protocol
	//todo

	//create objects in SimulationInterfaceFactory
	for (std::string interfacename : *interfacenames) {
		iSimulationData* iData = SimulationInterfaceFactory::makeInterface(interfacename);
		//set parameters of config
		reader.setConfig(iData->getMapper(), interfacename);
		simulationInterfaces.push_back(iData);
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
