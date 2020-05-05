#ifndef YAMLCONFIGREADER_H
#define YAMLCOFIGREADER_H

#include <vector>
#include <string>
#include "yaml-cpp/yaml.h"
#include "mapper/Mapper.h"
#include "configreader/StandardYAMLConfig.h"

class YAMLConfigReader {
public:
	YAMLConfigReader(std::string path);
	
	std::vector<std::string>* getSimulatorNames();
	int setConfig(Mapper* mapper, std::string simulatorName);

private:
	std::string path;
};

#endif // !YAMLCONFIGREADER_H