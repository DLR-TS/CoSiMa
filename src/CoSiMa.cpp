﻿#include "CoSiMa.h"
#include <filesystem>

int main(int argc, char *argv[])
{
	std::cout << "Welcome to CoSiMa." << std::endl << std::endl;

	std::cout << std::filesystem::current_path() << std::endl << std::endl;

	//start parameter
	std::string path(std::filesystem::current_path().string());
	path.append("\\config.yaml");
	for (int i = 1; i < argc; i++) {
		std::string currentArg = argv[i];
		path = currentArg;//add more complex evaluation if necessary
	}

	//read config
	YAMLConfigReader reader = YAMLConfigReader(path);
	const std::vector<SingleYAMLConfig> simulatornames = reader.getSimulatorNames();

	bool carlaUsedAsBaseInterface = false;
	std::shared_ptr<BaseSystemInterface> baseSystem;
	/**
	* Vector that holds every simulation interface.
	*/
	std::vector<std::shared_ptr<iSimulationData>> simulationInterfaces;
	//create objects in SimulationInterfaceFactory
	for (SingleYAMLConfig simulatorname : simulatornames) {
		if (simulatorname.simulator == CARLA) {
			carlaUsedAsBaseInterface = true;
			baseSystem = std::make_shared<CARLAInterface>();
			if (reader.setBaseSystemConfig(baseSystem, simulatorname)) {
				std::cout << "Problem occured during interpretation of configuration file. (Base System)" << std::endl;
				exit(5);
			}
			continue;
		}

		std::shared_ptr<iSimulationData> newInterface = SimulationInterfaceFactory::makeInterface(simulatorname.simulator);
		if (newInterface == nullptr) {
			std::cout << "Failed to create a simulator." << std::endl;
			exit(1);
		}
		//set parameters of config
		if (reader.setConfig(newInterface, simulatorname)) {
			std::cout << "Problem occured during interpretation of configuration file. (Interfaces)" << std::endl;
			exit(2);
		}
		simulationInterfaces.push_back(newInterface);
	}

	if (!carlaUsedAsBaseInterface) {
		baseSystem = std::make_shared<DominionInterface>();
	}


	//init interfaces
	if (0 != baseSystem->initialise()) {
		std::cerr << "Error in initialization of base simulation interface." << std::endl;
		exit(6);
	}
	for (auto simInterface : simulationInterfaces) {
		if (simInterface->init("Scenario", 0.0, 0) != 0) { //TODO set as parameters?
			std::cout << "Error in initialization of simulation interfaces." << std::endl;
			exit(3);
		}
	}

	//connect interfaces
	for (auto simInterface : simulationInterfaces) {
		if (simInterface->connect("")) { //TODO set as parameters?
			std::cout << "Error in connect of simulation interfaces." << std::endl;
			exit(4);
		}
	}

	simulationLoop(simulationInterfaces, baseSystem);

	//disconnect interfaces
	for (auto simInterface : simulationInterfaces) {
		if (simInterface->disconnect()) {
			std::cout << "Error in disconnect of simulation interfaces." << std::endl;
		}
	}
	return 0;
}

void simulationLoop(std::vector<std::shared_ptr<iSimulationData>> &simulationInterfaces, std::shared_ptr <BaseSystemInterface> &baseSystem) {
	//start simulationloop
	bool continueSimulationLoop = true;

	while (continueSimulationLoop) {

		//read from base_system
		for (auto &simInterface : simulationInterfaces) {
			//read from baseSystem, sort in internalState
			if (simInterface->mapToInterfaceSystem(baseSystem)) {
				std::cout << "Error in input matching while updating internal state." << std::endl;
				continueSimulationLoop = false;
			}
			//write to interface
			if (simInterface->readFromInternalState()) {
				std::cout << "Error in input matching while updating simulation interface inputs." << std::endl;
				continueSimulationLoop = false;
			}
		}

		for (auto &simInterface : simulationInterfaces) {
			//do simulaton step
			simInterface->doStep();
		}
		// base simulation interface also performs a step, at least to update its clock
		baseSystem->doStep();

		for (auto &simInterface : simulationInterfaces) {
			//get output data from interface and sort into internalState
			simInterface->writeToInternalState();
			//and write to base system
			simInterface->mapFromInterfaceSystem(baseSystem);
		}
	}
}