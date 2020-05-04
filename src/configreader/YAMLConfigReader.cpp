#include <string>
#include "configreader/YAMLConfigReader.h"

YAMLConfigReader::YAMLConfigReader(std::string path) {
	this->path = path;
}

std::vector<std::string>* YAMLConfigReader::getInterfaceNames() {
	YAML::Node simulators = YAML::LoadFile(path);
	std::vector<std::string> *names = new std::vector<std::string>();

	for (std::size_t i = 0; i < simulators.size(); i++) {
		InterfaceYAMLConfig conf = simulators[i].as<InterfaceYAMLConfig>();
		names->push_back(conf.simulator);
	}
	return names;
}

void YAMLConfigReader::setConfig(Mapper* mapper, std::string name) {
	YAML::Node simulators = YAML::LoadFile(path);
	for (std::size_t i = 0; i < simulators.size(); i++) {
		InterfaceYAMLConfig conf = simulators[i].as<InterfaceYAMLConfig>();
		if (conf.simulator.compare(name) == 0) {
			mapper->readConfiguration(conf);
			break;
		}
	}
}