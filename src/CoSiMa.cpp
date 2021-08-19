#include "CoSiMa.h"
#include <filesystem>
#include <fstream>
#include "base_interfaces/CARLAInterface.h"

int main(int argc, char *argv[])
{
	cmdParameter runtimeParameter;

	std::cout << "Welcome to CoSiMa." << std::endl << std::endl;
#if __cplusplus > 201703L
	std::cout << std::filesystem::current_path() << std::endl << std::endl;

	//start parameter
	std::string path(std::filesystem::current_path().string());
#else
	std::string path;
#endif
	for (int i = 1; i < argc; i++) {
		std::string currentArg = argv[i];
		if (currentArg == "-d") {
			runtimeParameter.debug = true;
		}
		else if (currentArg == "-l") {
			runtimeParameter.log = true;
		}
		else if (currentArg == "-OSI") {
			runtimeParameter.logOSI = true;
			std::cout << "You started CoSiMa with OSI. This will generate large output, since OSI messages can be very large.\nWrite Y to proceed an N to quit.\n";
			std::string in;
			std::cin >> in;
			if (in != "Y" && in != "y")
			{
				std::cout << "Close CoSiMa" << std::endl;
				exit(0);
			}
		}
		else {
			path = std::move(currentArg);//add more complex evaluation if necessary
		}
	}

	//Logging
	std::ofstream out;

	if (runtimeParameter.log) {
		out = std::ofstream("cosima.log");
		std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout
	}

	if (runtimeParameter.debug) {
		std::cout << "Path: " << path << "\n";
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
			std::cout << "Add CARLA module" << std::endl;
			carlaUsedAsBaseInterface = true;
			baseSystem = std::make_shared<CARLAInterface>();
			if (reader.setBaseSystemConfig(baseSystem, simulatorname)) {
				std::cout << "Problem occured during interpretation of configuration file. (Base System)" << std::endl;
				exit(5);
			}
			continue;
		}

		std::shared_ptr<iSimulationData> newInterface = SimulationInterfaceFactory::makeInterface(simulatorname.simulator, runtimeParameter.debug);
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

	if (runtimeParameter.debug) {
		std::cout << "Begin Initializiation \n" << std::endl;
	}

	//init interfaces
	if (0 != baseSystem->initialise(runtimeParameter.debug, runtimeParameter.logOSI)) {
		std::cerr << "Error in initialization of base simulation interface." << std::endl;
		exit(6);
	}
	for (auto simInterface : simulationInterfaces) {
		if (simInterface->init("Scenario", 0.0, 0) != 0) { //TODO set as parameters?
			std::cout << "Error in initialization of simulation interfaces." << std::endl;
			exit(3);
		}
	}

	if (runtimeParameter.debug) {
		std::cout << "End Initializiation \nBegin Connect Phase\n " << std::endl;
	}

	//connect interfaces
	for (auto simInterface : simulationInterfaces) {
		if (simInterface->connect("")) { //TODO set as parameters?
			std::cout << "Error in connect of simulation interfaces." << std::endl;
			exit(4);
		}
	}

	if (runtimeParameter.debug) {
		std::cout << "End Connect Phase \nBegin Simulation Loops\n " << std::endl;
	}

	simulationLoop(simulationInterfaces, baseSystem, runtimeParameter);

	if (runtimeParameter.debug) {
		std::cout << "Begin Disconnect Phase\n " << std::endl;
	}

	//disconnect interfaces
	for (auto simInterface : simulationInterfaces) {
		if (simInterface->disconnect()) {
			std::cout << "Error in disconnect of simulation interfaces." << std::endl;
		}
	}
	return 0;
}

void simulationLoop(std::vector<std::shared_ptr<iSimulationData>> &simulationInterfaces, std::shared_ptr <BaseSystemInterface> &baseSystem, const cmdParameter& runtimeParameter) {
	//start simulationloop
	bool continueSimulationLoop = true;

	while (continueSimulationLoop) {

		//read from base_system
		if (runtimeParameter.debug) {
			std::cout << "Read Base System and write to Interfaces Phase\n";
		}
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

		if (runtimeParameter.debug) {
			std::cout << "DoStep Phase\n";
		}

		for (auto &simInterface : simulationInterfaces) {
			//do simulaton step
			simInterface->doStep();
		}
		// base simulation interface also performs a step, at least to update its clock
		baseSystem->doStep();

		if (runtimeParameter.debug) {
			std::cout << "Read Interfaces Phase\n";
		}
		for (auto &simInterface : simulationInterfaces) {
			//get output data from interface and sort into internalState
			simInterface->writeToInternalState();
			if (runtimeParameter.logOSI) {
				//log all String entries of interface
				for (std::string& osi_message : simInterface->getMapper()->getInternalState()->strings) {
					std::cout << "Message:" << osi_message << "\n";
				}
			}
			//and write to base system
			simInterface->mapFromInterfaceSystem(baseSystem);
		}
	}
}
