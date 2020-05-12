#include "CoSiMa.h"
#include "SimulationInterfaceFactory.h"

int main(int argc, char *argv[])
{
	std::cout << "Welcome to CoSiMa." << std::endl << std::endl;

	//start parameter
	std::string path("D:/config.yaml");
	for (int i = 1; i < argc; i++) {
		std::string currentArg = argv[i];
		path = currentArg;//add more complex evaluation if necessary
	}

	//read config
	YAMLConfigReader reader = YAMLConfigReader(path);
	const std::vector<eSimulatorName> simulatornames = reader.getSimulatorNames(); // TODO: write unit test for reader.getSimulatorNames
	
	//choose protocol
	//TODO

	/**
	* Vector that holds every simulation interface.
	*/
	std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces;
	//create objects in SimulationInterfaceFactory
	for (eSimulatorName simulatorname : simulatornames) {
		std::shared_ptr<iSimulationData> newInterface = SimulationInterfaceFactory::makeInterface(simulatorname);
		//set parameters of config
		if (reader.setConfig(newInterface, simulatorname)) {
			std::cout << "Problem occured during interpretation of configuration file." << std::endl;
			return 1;
		}
		simulationInterfaces.push_back(newInterface);
	}

	//init interfaces
	for (auto simInterface : simulationInterfaces) {
		simInterface->init("Scenario", 0.0, 0); //TODO set as parameters?
	}

	//start simulationloop
	bool continueSimulationLoop = true;

	while (continueSimulationLoop) {
		for (auto simInterface : simulationInterfaces) {
			//read from internalstate and write to interface
			if (simInterface->mapInput(simulationInterfaces)) {
				std::cout << "Error in input matching." << std::endl;
				continueSimulationLoop = false;
			}
		}

		for (auto simInterface : simulationInterfaces) {
			//do simulaton step
			simInterface->doStep();
		}

		for (auto simInterface : simulationInterfaces) {
			//get output data from interface and write to internalState
			simInterface->readOutputs();
		}
	}
	return 0;
}
