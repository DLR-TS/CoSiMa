#include "CoSiMa.h"

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
		if (currentArg == "-d" || currentArg == "-v") {
			runtimeParameter.verbose = true;
		}
		else if (currentArg == "-l") {
			runtimeParameter.log = true;
			runtimeParameter.logPath = std::string(argv[++i]);
		}
		else if (currentArg == "-t") {
			runtimeParameter.timestamps = true;
		}
		else if (currentArg == "-OSI" || currentArg == "-osi") {
			runtimeParameter.logOSI = true;
			std::cout << "You started CoSiMa with OSI messages activated in output. This will generate large output, since OSI messages can be very large.\nWrite Y to proceed an N to quit.\n";
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
		std::cout << "No more output on the console. Output is directed to " << runtimeParameter.logPath << std::endl;
		out = std::ofstream(runtimeParameter.logPath);
		std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout
		std::cout << "##########" << "COSIMALOG" << "##########" << std::endl;
	}

	if (runtimeParameter.verbose) {
		std::cout << "Configuration file: " << path << "\n";
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

		std::shared_ptr<iSimulationData> newInterface = SimulationInterfaceFactory::makeInterface(simulatorname.simulator, runtimeParameter.verbose);
		if (newInterface == nullptr) {
			std::cout << "Failed to create a simulator interface." << std::endl;
			exit(1);
		}
		//set parameters of config
		if (reader.setConfig(*newInterface, simulatorname)) {
			std::cout << "Problem occured during interpretation of configuration file. (Interfaces)" << std::endl;
			exit(2);
		}
		simulationInterfaces.push_back(std::move(newInterface));
	}

	if (runtimeParameter.verbose) {
		std::cout << "Begin Initializiation \n" << std::endl;
	}

	//init interfaces
	if (0 != baseSystem->initialize(runtimeParameter.verbose, runtimeParameter.logOSI)) {
		std::cerr << "Error in initialization of base simulation interface." << std::endl;
		exit(6);
	}
	for (auto &simInterface : simulationInterfaces) {
		if (simInterface->init(0.0) != 0) { //TODO
			std::cout << "Error in initialization of simulation interfaces." << std::endl;
			exit(3);
		}
	}

	if (runtimeParameter.verbose) {
		std::cout << "End Initializiation \nBegin Connect Phase\n " << std::endl;
	}

	simulationLoop(simulationInterfaces, baseSystem, runtimeParameter);

	if (runtimeParameter.verbose) {
		std::cout << "Begin Disconnect Phase\n " << std::endl;
	}

	//disconnect interfaces
	for (auto &simInterface : simulationInterfaces) {
		if (simInterface->disconnect()) {
			std::cout << "Error in disconnect of simulation interfaces." << std::endl;
		}
	}
	//base system disconnect
	baseSystem->disconnect();
	return 0;
}

inline void printTimeStamp(bool printtimestamps) {
	if (printtimestamps) {
		auto time = std::chrono::system_clock::now().time_since_epoch();
		auto millisec_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
		auto sec_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(time).count();

		std::cout << "Time:" << sec_since_epoch << ":" << millisec_since_epoch << "\n";
	}
}

void simulationLoop(std::vector<std::shared_ptr<iSimulationData>> &simulationInterfaces,
	std::shared_ptr <BaseSystemInterface> &baseSystem, const cmdParameter& runtimeParameter) {
	//start simulationloop
	bool continueSimulationLoop = true;

	double stepsize = baseSystem->getStepSize();
	double total_time = 0;

	while (continueSimulationLoop) {

		//read from base_system
		if (runtimeParameter.verbose) {
			std::cout << "Write information to all interfaces" << std::endl;
		}
		for (auto &simInterface : simulationInterfaces) {
			//read from baseSystem, sort in internalState
			printTimeStamp(runtimeParameter.timestamps);
			if (simInterface->mapToInterfaceSystem(baseSystem)) {
				std::cout << "Error in input matching while updating internal state." << std::endl;
				continueSimulationLoop = false;
			}
			printTimeStamp(runtimeParameter.timestamps);
			//write to interface
			if (simInterface->readFromInternalState()) {
				std::cout << "Error in input matching while updating simulation interface inputs." << std::endl;
				continueSimulationLoop = false;
			}
			printTimeStamp(runtimeParameter.timestamps);
		}

		if (runtimeParameter.verbose) {
			std::cout << "Modules DoStep with stepsize: " << stepsize << " Simulation Time: " << total_time << std::endl;
		}
		total_time += stepsize;

		for (auto &simInterface : simulationInterfaces) {
			//do simulaton step
			printTimeStamp(runtimeParameter.timestamps);
			simInterface->doStep(stepsize);
			printTimeStamp(runtimeParameter.timestamps);
		}
		// base simulation interface also performs a step
		if (runtimeParameter.verbose) {
			std::cout << "BaseSystem do step" << std::endl;
		}
		printTimeStamp(runtimeParameter.timestamps);
		baseSystem->doStep(stepsize);
		printTimeStamp(runtimeParameter.timestamps);

		if (runtimeParameter.verbose) {
			std::cout << "Read information from all interfaces" << std::endl;
		}
		for (auto &simInterface : simulationInterfaces) {
			//get output data from interface and sort into internalState
			printTimeStamp(runtimeParameter.timestamps);
			simInterface->writeToInternalState();
			printTimeStamp(runtimeParameter.timestamps);
			//and write to base system
			simInterface->mapFromInterfaceSystem(baseSystem);
			printTimeStamp(runtimeParameter.timestamps);
		}
	}
}
