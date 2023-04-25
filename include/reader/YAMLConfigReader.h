/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef YAMLCONFIGREADER_H
#define YAMLCOFIGREADER_H

#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

#include "yaml-cpp/yaml.h"
#include "mapper/Mapper.h"
#include "base_interfaces/BaseSystemInterface.h"
#include "base_interfaces/DummyInterface.h"
#include "base_interfaces/CARLAInterface.h"
#include "simulation_interfaces/OSMPInterface.h"
#include "reader/StandardYAMLConfig.h"

struct SimulationSetup {
	std::shared_ptr<BaseSystemInterface> baseSimulator;
	std::vector<std::shared_ptr<SimulatorInterface>> childSimulators;
	bool valid = true;
};

/**
* Load configuration file.
* \param path Path to yaml configuration file.
*/
YAML::Node loadConfigurationFile(const std::string& path);

/**
* Retrieve the names of all simulators.
* \param node YAML Node.
* \return parsed configuration of yaml file
*/
SimulationSetup parseSimulationConfiguration(YAML::Node& node);

/**
* Converts simulatornames to enum values.
* \param simulatorName string representation of simulator name.
* \return Enum representation of simulator name.
*/
const eSimulatorTypes nameToEnum(std::string simulatorName);

#endif // !YAMLCONFIGREADER_H
