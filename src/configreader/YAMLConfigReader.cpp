#include "configreader/YAMLConfigReader.h"
#include <string>
#include <algorithm>

YAMLConfigReader::YAMLConfigReader(std::string path) {
	std::cout << "Load YAML file: " << path << std::endl;
	simulators = YAML::LoadFile(path);
}

const std::vector<SingleYAMLConfig> YAMLConfigReader::getSimulatorNames() {
	std::vector<SingleYAMLConfig> names;
	//counter for later tracking the amount of same interfaces with different configurations
	std::map<eSimulatorName, int> counter;
	for (int i = FMI; i != SIMULATORNAME_ERROR; i++)
	{
		eSimulatorName name = static_cast<eSimulatorName>(i);
		counter.emplace(name, 0);
	}

	for (std::size_t i = 0; i < simulators.size(); i++) {
		SimulatorName conf = simulators[i].as<SimulatorName>();
		const eSimulatorName simName = nameToEnum(conf.simulator);
		if (simName == SIMULATORNAME_ERROR) {
			std::cout << "Not supported simulator name in yaml configration file: " << conf.simulator << std::endl;
			exit(1);
		}
		std::map<eSimulatorName, int>::iterator countIter = counter.find(simName);
		names.push_back(SingleYAMLConfig(simName, countIter->second));
		countIter->second++;
	}
  std::cout << "Connecting " << names.size() << " modules." << std::endl;
	return names;
}

int YAMLConfigReader::setConfig(std::shared_ptr<iSimulationData> simulator, SingleYAMLConfig simulatorname) {
	int index = 0;
	for (std::size_t i = 0; i < simulators.size(); i++) {
		SimulatorName name = simulators[i].as<SimulatorName>();
		if (nameToEnum(name.simulator) == simulatorname.simulator) {
			if (index == simulatorname.index) {
				switch (simulatorname.simulator) {
				case DEFAULT:
					std::cout << "Add DEFAULT module" << std::endl;
					return simulator->getMapper()->readConfiguration(simulators[i].as<InterfaceYAMLConfig>());
				case FMI:
					std::cout << "Add FMI module" << std::endl;
					return simulator->getMapper()->readConfiguration(simulators[i].as<FMIInterfaceConfig>());
				case OSI:
					std::cout << "Add OSI module" << std::endl;
					return simulator->getMapper()->readConfiguration(simulators[i].as<OSIInterfaceConfig>());
				case OSMP:
					std::cout << "Add OSMP module" << std::endl;
					return simulator->getMapper()->readConfiguration(simulators[i].as<OSMPInterfaceConfig>());
				}
			}
			else {
				index++;
			}
		}
	}
	std::cout << "Error no node found with name: " << simulatorname.simulator << std::endl;
	return 1;
}

int YAMLConfigReader::setBaseSystemConfig(std::shared_ptr<BaseSystemInterface> baseSystem, SingleYAMLConfig simulatorname) {
	for (std::size_t i = 0; i < simulators.size(); i++) {
		SimulatorName name = simulators[i].as<SimulatorName>();
		if (nameToEnum(name.simulator) == simulatorname.simulator) {
			if (simulatorname.simulator == CARLA) {
				return baseSystem->readConfiguration(simulators[i].as<CARLAInterfaceConfig>());
			}
		}
	}
	std::cout << "Error no node found with name: " << simulatorname.simulator << std::endl;
	return 1;
}

const eSimulatorName YAMLConfigReader::nameToEnum(std::string simulatorName) {
	//compare lower case 
	std::transform(simulatorName.begin(), simulatorName.end(), simulatorName.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (simulatorName == "carla") {
		return CARLA;
	}
	else if (simulatorName == "fmi") {
		return FMI;
	}
	else if (simulatorName == "default") {
		return DEFAULT;
	}
	else if (simulatorName == "osi") {
		return OSI;
	}
	else if (simulatorName == "osmp") {
		return OSMP;
	}
	else {
		return SIMULATORNAME_ERROR;
	}
}
