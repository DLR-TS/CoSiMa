#ifndef YAMLCONFIGREADER_H
#define YAMLCOFIGREADER_H

#include <vector>
#include <string>
#include "yaml-cpp/yaml.h"
#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "configreader/StandardYAMLConfig.h"

class YAMLConfigReader {
public:
	YAMLConfigReader(std::string path);
	
	std::vector<SupportedInterfaces>* getSimulatorNames();
	int setConfig(Mapper* mapper, SupportedInterfaces simulator);

private:
	std::string path;

	SupportedInterfaces nameToEnum(std::string simulatorName);
};

#endif // !YAMLCONFIGREADER_H