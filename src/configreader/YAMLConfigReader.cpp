#include <string>
#include "configreader/YAMLConfigReader.h"

YAMLConfigReader::YAMLConfigReader(std::string path) {
	this->path = path;
}

std::vector<std::string>* YAMLConfigReader::getSimulatorNames() {
	YAML::Node simulators = YAML::LoadFile(path);
	std::vector<std::string> *simulatorNames = new std::vector<std::string>();

	for (std::size_t i = 0; i < simulators.size(); i++) {
		InterfaceYAMLConfig conf = simulators[i].as<InterfaceYAMLConfig>();
		simulatorNames->push_back(conf.simulator);
	}
	return simulatorNames;
}

int YAMLConfigReader::setConfig(Mapper* mapper, std::string simulator) {
	YAML::Node simulators = YAML::LoadFile(path);
	for (std::size_t i = 0; i < simulators.size(); i++) {
		InterfaceYAMLConfig conf = simulators[i].as<InterfaceYAMLConfig>();
		if (conf.simulator.compare(simulator) == 0) {
			return mapper->readConfiguration(conf);
		}
	}
	std::cout << "Error no node found with name: " << simulator << std::endl;
	return 1;
}