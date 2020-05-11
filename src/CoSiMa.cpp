#include "CoSiMa.h"
#include "SimulationInterfaceFactory.h"

int main(int argc, char *argv[])
{
	std::string path("D:/config.yaml");
	for (int i = 1; i < argc; ++i) {
		std::string currentArg = argv[i];
		path = currentArg;//add more complex evaluation if necessary
	}
	std::cout << "Welcome to CoSiMa." << std::endl;


	//read config
	YAMLConfigReader reader = YAMLConfigReader(path);
	const std::vector<SupportedInterfaces> simulatornames = reader.getSimulatorNames(); // TODO: write unit test for reader.getSimulatorNames
	//choose protocol
	//todo

	//create objects in SimulationInterfaceFactory
	for (SupportedInterfaces simulator : simulatornames) {
		iSimulationData* iData = SimulationInterfaceFactory::makeInterface(simulator);
		//set parameters of config
		if (reader.setConfig(iData->getMapper(), simulator)) {
			std::cout << "Problem occured during interpretation of configuration file." << std::endl;
			return 1;
		}
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
