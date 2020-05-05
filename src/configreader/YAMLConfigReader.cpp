#include <string>
#include "configreader/YAMLConfigReader.h"

YAMLConfigReader::YAMLConfigReader(std::string path) {
	this->path = path;
}

std::vector<SupportedInterfaces>* YAMLConfigReader::getSimulatorNames() {
	YAML::Node simulators = YAML::LoadFile(path);
	std::vector<SupportedInterfaces> *simulatorNames = new std::vector<SupportedInterfaces>();

	for (std::size_t i = 0; i < simulators.size(); i++) {
		SimulatorName conf = simulators[i].as<SimulatorName>();
		simulatorNames->push_back(nameToEnum(conf.simulator));
	}
	return simulatorNames;
}

int YAMLConfigReader::setConfig(Mapper* mapper, SupportedInterfaces simulator) {
	YAML::Node simulators = YAML::LoadFile(path);
	for (std::size_t i = 0; i < simulators.size(); i++) {
		SimulatorName name = simulators[i].as<SimulatorName>();
		if (nameToEnum(name.simulator) == simulator) {
			switch (simulator) {
			case VTD:
			case SUMO:
			case UNREAL:
			case ROS:
				return mapper->readConfiguration(simulators[i].as<InterfaceYAMLConfig>());
			case FMI:
				return ((FMIMapper*)mapper)->readConfiguration(simulators[i].as<FMIInterfaceConfig>());
			//case OSI:
				//todo
			}

		}
	}
	std::cout << "Error no node found with name: " << simulator << std::endl;
	return 1;
}

SupportedInterfaces YAMLConfigReader::nameToEnum(std::string simulatorName) {
	if (simulatorName == "VTD" || simulatorName == "vtd") {
		return VTD;
	}
	else if (simulatorName == "FMI" || simulatorName == "fmi") {
		return FMI;
	}
	else if (simulatorName == "SUMO" || simulatorName == "sumo") {
		return SUMO;
	}
	else if (simulatorName == "OSI" || simulatorName == "osi") {
		return OSI;
	}
	else if (simulatorName == "UE" || simulatorName == "UnrealEngine" || simulatorName == "Unreal" || simulatorName == "UNREAL") {
		return UNREAL;
	}
	else if (simulatorName == "ROS" || simulatorName == "ros") {
		return ROS;
	}
}