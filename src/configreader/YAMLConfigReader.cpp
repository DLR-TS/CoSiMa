#include <string>
#include "configreader/YAMLConfigReader.h"

YAMLConfigReader::YAMLConfigReader(std::string path) {
	std::cout << "Load YAML file: " << path << std::endl;
	simulators = YAML::LoadFile(path);
}

const std::vector<eSimulatorName> YAMLConfigReader::getSimulatorNames() {
	std::vector<eSimulatorName> simulatorNames; 

	for (std::size_t i = 0; i < simulators.size(); i++) {
		SimulatorName conf = simulators[i].as<SimulatorName>();
		eSimulatorName simName = nameToEnum(conf.simulator);
		if (simName == SIMULATORNAME_ERROR) {
			std::cout << "Not supported simulator name in yaml configration file: " << conf.simulator << std::endl;
			exit(1);
		}
		simulatorNames.push_back(simName);
	}
	return simulatorNames;
}

int YAMLConfigReader::setConfig(std::shared_ptr<iSimulationData> simulator, eSimulatorName simulatorname) {
	for (std::size_t i = 0; i < simulators.size(); i++) {
		SimulatorName name = simulators[i].as<SimulatorName>();
		if (nameToEnum(name.simulator) == simulatorname) {
			switch (simulatorname) {
			case VTD:
			case SUMO:
			case UNREAL:
			case ROS:
				return simulator->getMapper()->readConfiguration(simulators[i].as<InterfaceYAMLConfig>());
			case FMI:
				return simulator->getMapper()->readConfiguration(simulators[i].as<FMIInterfaceConfig>());
			//case OSI:
				//TODO
			}

		}
	}
	std::cout << "Error no node found with name: " << simulatorname << std::endl;
	return 1;
}

eSimulatorName YAMLConfigReader::nameToEnum(std::string simulatorName) {
	std::transform(simulatorName.begin(), simulatorName.end(), simulatorName.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (simulatorName == "vtd") {
		return VTD;
	}
	else if (simulatorName == "fmi") {
		return FMI;
	}
	else if (simulatorName == "sumo") {
		return SUMO;
	}
	else if (simulatorName == "osi") {
		return OSI;
	}
	else if (simulatorName == "ue" || simulatorName == "unrealengine" || simulatorName == "unreal") {
		return UNREAL;
	}
	else if (simulatorName == "ros") {
		return ROS;
	}
	else {
		return SIMULATORNAME_ERROR;
	}
}
