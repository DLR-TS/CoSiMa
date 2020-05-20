﻿#include "CoSiMa.h"
#include <filesystem>

int main(int argc, char *argv[])
{
	std::cout << "Welcome to CoSiMa." << std::endl << std::endl;

	std::cout << std::filesystem::current_path() << std::endl << std::endl;

	//start parameter
	std::string path("../config.yaml");
	for (int i = 1; i < argc; i++) {
		std::string currentArg = argv[i];
		path = currentArg;//add more complex evaluation if necessary
	}

	//read config
	YAMLConfigReader reader = YAMLConfigReader(path);
	const std::vector<SingleYAMLConfig> simulatornames = reader.getSimulatorNames();
	
	/**
	* Vector that holds every simulation interface.
	*/
	std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces;
	//create objects in SimulationInterfaceFactory
	for (SingleYAMLConfig simulatorname : simulatornames) {
		std::shared_ptr<iSimulationData> newInterface = SimulationInterfaceFactory::makeInterface(simulatorname.simulator);
		if (newInterface == nullptr){
			std::cout << "Failed to create a simulator." << std::endl;
			exit(1);
		}
		//set parameters of config
		if (reader.setConfig(newInterface, simulatorname)) {
			std::cout << "Problem occured during interpretation of configuration file." << std::endl;
			return 1;
		}
		simulationInterfaces.push_back(newInterface);
	}

	//choose protocol
	//TODO
	std::shared_ptr<BaseSystemInterface> baseSystem = std::shared_ptr<BaseSystemInterface>((BaseSystemInterface*) new DominionInterface());


	//init interfaces
	for (auto simInterface : simulationInterfaces) {
		simInterface->init("Scenario", 0.0, 0); //TODO set as parameters?
	}

	simulationLoop(simulationInterfaces, baseSystem);
	return 0;
}

void simulationLoop(std::vector<std::shared_ptr<iSimulationData>> &simulationInterfaces, std::shared_ptr <BaseSystemInterface> &baseSystem) {
	//start simulationloop
	bool continueSimulationLoop = true;

	while (continueSimulationLoop) {

		//read from base_system
		for (auto &simInterface : simulationInterfaces) {
			//read from baseSystem, sort in internalState and write to interface
			if (simInterface->mapInput(baseSystem)) {
				std::cout << "Error in input matching." << std::endl;
				continueSimulationLoop = false;
			}
		}

		for (auto &simInterface : simulationInterfaces) {
			//do simulaton step
			simInterface->doStep();
		}

		for (auto &simInterface : simulationInterfaces) {
			//get output data from interface and sort into internalState
			simInterface->readOutputs();
			//and write to base system
			simInterface->writeTo(baseSystem);
		}
	}
}