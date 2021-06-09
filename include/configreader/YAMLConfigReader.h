/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef YAMLCONFIGREADER_H
#define YAMLCOFIGREADER_H

#include <vector>
#include <string>
#include "yaml-cpp/yaml.h"
#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "configreader/StandardYAMLConfig.h"

class SingleYAMLConfig {
public:
	SingleYAMLConfig() {};
	SingleYAMLConfig(const eSimulatorName name, int index) {
		this->index = index;
		this->simulator = name;
	};

	int index;
	eSimulatorName simulator;
};

class YAMLConfigReader {
public:
	/**
	* Constructor of YAMLConfigReader
	* \param path Path to yaml configuration file.
	*/
	YAMLConfigReader(std::string path);
	/**
	* Retrieve the names of all simulators.
	* \return const vector of interfaces.
	*/
	const std::vector<SingleYAMLConfig> getSimulatorNames();
	/**
	* Set config of simulator to given mapper.
	* \param simulator Mapper to be configured.
	* \param simulatorname Read simulator configuration of this name.
	*/
	int setConfig(std::shared_ptr<iSimulationData> simulator, SingleYAMLConfig simulatorname);

	/**
	* Set config of simulator to given mapper.
	* \param baseSystem BaseSystem to be configured.
	* \param simulatorname Read simulator configuration of this name.
	*/
	int setBaseSystemConfig(std::shared_ptr<BaseSystemInterface> baseSystem, SingleYAMLConfig simulatorname);

private:
	/**
	* Representation of the YAML configuration file.
	*/
	YAML::Node simulators;

	/**
	* Converts simulatornames to enum values.
	* \param simulatorName String representation of simulator name.
	* \return Enum representation of simulator name.
	*/
	const eSimulatorName nameToEnum(std::string simulatorName);
};

#endif // !YAMLCONFIGREADER_H
