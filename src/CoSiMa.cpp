#include "CoSiMa.h"

CmdParameter parseRuntimeParameter(int argc, char *argv[]) {
	CmdParameter runtimeParameter;
	for (int i = 1; i < argc; i++) {
		std::string currentArg(argv[i]);
		if (currentArg == "-d" || currentArg == "-v") {
			runtimeParameter.verbose = true;
		}
		if (currentArg == "-p") {
			runtimeParameter.parallel = true;
		}
		if (currentArg == "-svc") {
			runtimeParameter.sensorviewconfiguration = true;
		}
		else {
			std::filesystem::path path(currentArg);
			runtimeParameter.configurationPath = path.parent_path().string();
			runtimeParameter.configurationName = path.filename();
		}
	}
	return runtimeParameter;
}

void Cosima::setRuntimeParameter(CmdParameter& runtimeParameter) {
	this->runtimeParameter = runtimeParameter;
}

void Cosima::loadConfiguration() {

	YAML::Node node = loadConfigurationFile(runtimeParameter.configurationPath + runtimeParameter.configurationName);
	if (node.IsNull()) {
		std::cout << "Error loading configuration with cpp-yaml" << std::endl;
		exit(0);
	}
	setup = parseSimulationConfiguration(node);
	if (!setup.valid)
	{
		std::cout << "Error parsing configuration" << std::endl;
		exit(0);
	}
}

void Cosima::initInterfaces() {
	if (runtimeParameter.verbose) {
		std::cout << "Begin Initializiation" << std::endl;
	}
	//init interfaces
	if (setup.baseSimulator->init(runtimeParameter.verbose)) {
		std::cout << "Error in initialization of base simulation interface." << std::endl;
		exit(0);
	}
	for (auto& simInterface : setup.childSimulators) {
		if (simInterface->init(runtimeParameter.verbose, runtimeParameter.configurationPath)) {
			std::cout << "Error in initialization of simulation interfaces." << std::endl;
			exit(0);
		}
	}
	if (runtimeParameter.verbose) {
		std::cout << "End Initializiation" << std::endl;
	}
}

void Cosima::sensorViewConfiguration() {
	if (runtimeParameter.verbose) {
		std::cout << "Begin SensorViewConfiguration" << std::endl;
	}
	for (auto simInterface : setup.childSimulators) {
		if (std::dynamic_pointer_cast<OSMPInterface>(simInterface)) {
			std::shared_ptr<OSMPInterface> osmpinterface = std::static_pointer_cast<OSMPInterface>(simInterface);
			std::string sensorViewConfig = osmpinterface->getSensorViewConfigurationRequest();
			if (sensorViewConfig != "") {
				osmpinterface->sensorviewindex = setup.baseSimulator->setOSIMessage("OSMPSensorViewConfigurationRequest", sensorViewConfig);
				std::string appliedSensorViewConfig = setup.baseSimulator->getOSIMessage("OSMPSensorViewConfiguration" + osmpinterface->sensorviewindex);
				osmpinterface->setSensorViewConfiguration(appliedSensorViewConfig);
			}
		}
	}
	if (runtimeParameter.verbose) {
		std::cout << "End SensorViewConfiguration" << std::endl;
	}
}

void Cosima::prepareSimulationStep(std::shared_ptr<SimulatorInterface> simInterface) {
	simInterface->mapToInterfaceSystem(setup.baseSimulator);
	simInterface->readFromInternalState();
}

void Cosima::doSimulationStep(std::shared_ptr<SimulatorInterface> simInterface) {
	simInterface->doStep(setup.baseSimulator->getStepSize());
}

void Cosima::postSimulationStep(std::shared_ptr<SimulatorInterface> simInterface) {
	int status = simInterface->writeToInternalState();
	if (status) {
		setup.baseSimulator->stopSimulation();
		return;
	}
	simInterface->mapFromInterfaceSystem(setup.baseSimulator);
}

void Cosima::stopSimulation(std::shared_ptr<SimulatorInterface> simInterface) {
	simInterface->stopSimulation();
}

void Cosima::simulationLoopParallel() {

	double total_time = 0;
	std::vector<std::thread> simulationThreads;

	while (!setup.baseSimulator->simulationStopped()) {

		for (auto &simInterface : setup.childSimulators) {
			simulationThreads.push_back(std::thread(&Cosima::prepareSimulationStep, this, simInterface));
		}
		for (auto &thread : simulationThreads) {
			thread.join();
		}
		simulationThreads.clear();

		if (runtimeParameter.verbose) {
			std::cout << "Modules DoStep with stepsize: " << setup.baseSimulator->getStepSize() << " Simulation Time: " << total_time << std::endl;
			total_time += setup.baseSimulator->getStepSize();
		}

		for (auto &simInterface : setup.childSimulators) {
			simulationThreads.push_back(std::thread(&Cosima::doSimulationStep, this, simInterface));
		}
		setup.baseSimulator->doStep(setup.baseSimulator->getStepSize());
		for (auto &thread : simulationThreads) {
			thread.join();
		}
		simulationThreads.clear();

		for (auto &simInterface : setup.childSimulators) {
			simulationThreads.push_back(std::thread(&Cosima::postSimulationStep, this, simInterface));
		}
		for (auto &thread : simulationThreads) {
			thread.join();
		}
		simulationThreads.clear();
	}
	for (auto &simInterface : setup.childSimulators) {
		simulationThreads.push_back(std::thread(&Cosima::stopSimulation, this, simInterface));
	}
	for (auto &thread : simulationThreads) {
		thread.join();
	}
	simulationThreads.clear();
}

void Cosima::simulationLoop() {
	if (runtimeParameter.parallel) {
		simulationLoopParallel();
		return;
	}

	double total_time = 0;

	while (!setup.baseSimulator->simulationStopped()) {

		if (runtimeParameter.verbose) {
			std::cout << "Modules DoStep with stepsize: " << setup.baseSimulator->getStepSize() << " Simulation Time: " << total_time << std::endl;
			total_time += setup.baseSimulator->getStepSize();
		}

		for (auto &simInterface : setup.childSimulators) {
			prepareSimulationStep(simInterface);
			doSimulationStep(simInterface);
			postSimulationStep(simInterface);
		}
		setup.baseSimulator->doStep(setup.baseSimulator->getStepSize());
	}
}


void Cosima::disconnect() {
	if (runtimeParameter.verbose) {
		std::cout << "Begin Disconnect Phase\n " << std::endl;
	}

	//disconnect interfaces
	for (auto &simInterface : setup.childSimulators) {
		if (simInterface->disconnect()) {
			std::cout << "Error in disconnect of simulation interfaces." << std::endl;
		}
	}
	//base system disconnect
	setup.baseSimulator->disconnect();
}
