#include "CoSiMa.h"

int main(int argc, char *argv[])
{
	cmdParameter runtimeParameter;
	std::string configurationPath;

	std::cout << "Welcome to CoSiMa.\n" << std::endl;

	for (int i = 1; i < argc; i++) {
		std::string currentArg = argv[i];
		if (currentArg == "-d" || currentArg == "-v") {
			runtimeParameter.verbose = true;
		}
		else {
			configurationPath = currentArg;
		}
	}

	if (configurationPath.size() == 0) {
		std::cout << "No yaml configuration file!" << std::endl;
		exit(0);
	}

	if (runtimeParameter.verbose) {
		std::cout << "Configuration file: " << configurationPath << std::endl;
	}

	Cosima cosima(runtimeParameter);
	cosima.loadConfiguration(configurationPath);
	cosima.initInterfaces();
	cosima.sensorViewConfiguration();
	cosima.simulationLoop();
	return 0;
}

void Cosima::loadConfiguration(std::string& configurationPath) {
	YAMLConfigReader reader = YAMLConfigReader(configurationPath);
	const std::vector<SingleYAMLConfig> simulatornames = reader.getSimulatorNames();

	//create objects in SimulationInterfaceFactory
	for (SingleYAMLConfig simulatorname : simulatornames) {
		if (simulatorname.simulator == CARLA) {
			std::cout << "Add CARLA module" << std::endl;
			baseSystem = std::make_shared<CARLAInterface>();
			if (reader.setBaseSystemConfig(baseSystem, simulatorname)) {
				std::cout << "Problem occured during interpretation of configuration file. (Base System)" << std::endl;
				exit(1);
			}
			continue;
		}

		std::shared_ptr<iSimulationData> newInterface = SimulationInterfaceFactory::makeInterface(simulatorname.simulator, runtimeParameter.verbose);
		if (newInterface == nullptr) {
			std::cout << "Failed to create a simulator interface." << std::endl;
			exit(2);
		}
		//set parameters of config
		if (reader.setConfig(*newInterface, simulatorname)) {
			std::cout << "Problem occured during interpretation of configuration file. (Interfaces)" << std::endl;
			exit(3);
		}
		simulationInterfaces.push_back(std::move(newInterface));
	}
}

void Cosima::initInterfaces() {
	if (runtimeParameter.verbose) {
		std::cout << "Begin Initializiation \n" << std::endl;
	}

	//init interfaces
	if (0 != baseSystem->initialize(runtimeParameter.verbose)) {
		std::cerr << "Error in initialization of base simulation interface." << std::endl;
		exit(4);
	}
	for (auto &simInterface : simulationInterfaces) {
		if (simInterface->init() != 0) {
			std::cout << "Error in initialization of simulation interfaces." << std::endl;
			exit(5);
		}
	}

	if (runtimeParameter.verbose) {
		std::cout << "End Initializiation \nBegin Connect Phase\n " << std::endl;
	}
}

void Cosima::sensorViewConfiguration() {

	for (auto simInterface : simulationInterfaces) {
		if (std::dynamic_pointer_cast<OSMPInterface>(simInterface)) {
			std::shared_ptr<OSMPInterface> osmpinterface = std::static_pointer_cast<OSMPInterface>(simInterface);
			std::string sensorViewConfig = osmpinterface->getSensorViewConfigurationRequest();
			osmpinterface->sensorviewindex = baseSystem->setStringValue("OSMPSensorViewConfigurationRequest", sensorViewConfig);
			std::string appliedSensorViewConfig = baseSystem->getStringValue("OSMPSensorViewConfiguration" + osmpinterface->sensorviewindex);
			osmpinterface->setSensorViewConfiguration(appliedSensorViewConfig);
		}
	}
}

void prepareSimulationStep(std::shared_ptr<iSimulationData> simInterface, std::shared_ptr<BaseSystemInterface> baseSystem) {
	simInterface->mapToInterfaceSystem(baseSystem);
	simInterface->readFromInternalState();
}

void doSimulationStep(std::shared_ptr<iSimulationData> simInterface, double stepsize) {
	simInterface->doStep(stepsize);
}

void postSimulationStep(std::shared_ptr<iSimulationData> simInterface, std::shared_ptr<BaseSystemInterface> baseSystem) {
	simInterface->writeToInternalState();
	simInterface->mapFromInterfaceSystem(baseSystem);
}

void Cosima::simulationLoop() {

	//start simulationloop
	bool continueSimulationLoop = true;
	double total_time = 0;
	double stepsize = baseSystem->getStepSize();
	std::vector<std::thread> simulationThreads;

	while (continueSimulationLoop) {

		for (auto &simInterface : simulationInterfaces) {
			simulationThreads.push_back(std::thread (prepareSimulationStep, simInterface, baseSystem));
		}
		for (auto &thread : simulationThreads) {
			thread.join();
		}
		simulationThreads.clear();

		if (runtimeParameter.verbose) {
			std::cout << "Modules DoStep with stepsize: " << stepsize << " Simulation Time: " << total_time << std::endl;
			total_time += stepsize;
		}

		for (auto &simInterface : simulationInterfaces) {
			simulationThreads.push_back(std::thread (doSimulationStep, simInterface, stepsize));
		}
		baseSystem->doStep(stepsize);
		for (auto &thread : simulationThreads) {
			thread.join();
		}
		simulationThreads.clear();

		for (auto &simInterface : simulationInterfaces) {
			simulationThreads.push_back(std::thread(postSimulationStep, simInterface, baseSystem));
		}
		for (auto &thread : simulationThreads) {
			thread.join();
		}
		simulationThreads.clear();
	}
}

void Cosima::disconnect() {
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
}
