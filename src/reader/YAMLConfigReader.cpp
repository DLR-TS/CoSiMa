#include "reader/YAMLConfigReader.h"

YAML::Node loadConfigurationFile(const std::string& path) {
	YAML::Node node;
	if (path.size() == 0) {
		std::cout << "No yaml configuration file given as a parameter!" << std::endl;
		return node;
	}
	std::cout << "Try to load YAML file: " << path << std::endl;
	if (!std::filesystem::exists(path)) {
		std::cout << "YAML file does not exist! Current path is " << std::filesystem::current_path() << std::endl;
		return node;
	}
	return YAML::LoadFile(path);
}

SimulationSetup parseSimulationConfiguration(YAML::Node& node) {
	SimulationSetup config;
	for (auto& nodechild : node) {
		SimulatorName name = nodechild.as<SimulatorName>();
		const eSimulatorTypes simName = nameToEnum(name.simulator);
		switch (simName) {
		case CARLA:
			config.baseSimulator = std::make_shared<CARLAInterface>();
			config.baseSimulator->configure(nodechild);
			break;
		case DUMMY:
			config.baseSimulator = std::make_shared<DummyInterface>();
			config.baseSimulator->configure(nodechild);
			break;
		case OSMP:
		{
			std::shared_ptr<OSMPInterface> osmp = std::make_shared<OSMPInterface>();
			osmp->configure(nodechild);
			config.childSimulators.push_back(osmp);
		}
		break;
		case SIMULATORNAME_ERROR:
			config.valid = false;
			break;
		}
	}
	if (!config.baseSimulator) {
		std::cout << "No base simulator in config defined. Examples are CARLA, DUMMY" << std::endl;
		config.valid = false;
	}
	return config;
}

const eSimulatorTypes nameToEnum(std::string simulatorName) {
	//compare lower case 
	std::transform(simulatorName.begin(), simulatorName.end(), simulatorName.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (simulatorName == "dummy" || simulatorName == "generic") {
		return DUMMY;
	}
	else if (simulatorName == "carla") {
		return CARLA;
	}
	else if (simulatorName == "osmp") {
		return OSMP;
	}
	else {
		std::cout << "Error parsing name: " << simulatorName << std::endl;
		return SIMULATORNAME_ERROR;
	}
}
