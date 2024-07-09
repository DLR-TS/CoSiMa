#include "CoSiMa.h"

CmdParameter Cosima::parseRuntimeParameter(int argc, char *argv[]) {

	for (int i = 1; i < argc; i++) {
		std::string currentArg(argv[i]);
		if (currentArg == "-d" || currentArg == "-v") {
			runtimeParameter.verbose = true;
		}
		else if (currentArg == "-p") {
			runtimeParameter.parallel = true;
		}
		else if (currentArg == "-sr") {
			runtimeParameter.scenarioRunner = true;
		}
		else {
			fs::path path(currentArg);
			runtimeParameter.configurationPath = path.parent_path().string();
			runtimeParameter.configurationName = path.filename().string();
		}
	}
	return runtimeParameter;
}

void Cosima::loadConfiguration() {

	YAML::Node node = loadConfigurationFile(runtimeParameter.configurationPath
		+ (runtimeParameter.configurationPath.size() ? "/" : "") + runtimeParameter.configurationName);
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

void Cosima::spawnLocalServices() {
	uint16_t port;
	if (setup.baseSimulator->isAutostart(port)) {
		subProcessController.spawnProcess(SPC_EXECUTABLE::CarlaOSISerivce, port, runtimeParameter.verbose);
	}

	uint16_t autoPort = 51430;
	for (auto& baseSimulator : setup.childSimulators) {
		if (baseSimulator->isAutostart(port)) {
			if (port == 0) {
				baseSimulator->setPort(autoPort);
				port = autoPort++;
			}
			baseSimulator->setPort(port);
			subProcessController.spawnProcess(SPC_EXECUTABLE::OSMPService, port, runtimeParameter.verbose);
		}
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Cosima::stopLocalServices() {
	subProcessController.shutdownProcesses();
}

void Cosima::waitForActiveScenarioRunner() {
	if (runtimeParameter.scenarioRunner) {
		srAdapter.init();
		srAdapter.waitForTick();
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
		//stepsize can change, so get it on every simulation step
		double stepSize = setup.baseSimulator->getStepSize();
		if (runtimeParameter.verbose && !runtimeParameter.scenarioRunner) {
			//total_time is wrong, if active scenario runner sets the timesteps
			std::cout << "Modules DoStep with stepsize: " << stepSize << " Simulation Time: " << total_time << std::endl;
			total_time += stepSize;
		}

		for (auto &simInterface : setup.childSimulators) {
			prepareSimulationStep(simInterface);
			doSimulationStep(simInterface);
			postSimulationStep(simInterface);
		}
		if (runtimeParameter.scenarioRunner) {
			//even if scenario runner does tick, this call to the base simulator must be made to update sensors etc.
			setup.baseSimulator->doStep(0);
			srAdapter.sendTickDone(stepSize);
			double tick = srAdapter.waitForTick();
			if (runtimeParameter.verbose) {
				std::cout << "Tick from scenario runner: " << tick << std::endl;
			}
			if (stepSize != tick) {
				std::cout << "Stepsize changed from " << stepSize << " to " << tick << std::endl;
			}
			setup.baseSimulator->setStepSize(tick);
		}
		else {
			setup.baseSimulator->doStep(stepSize);
		}
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
