/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef STANDARDYAMLCONGIF_H
#define STANDARDYAMLCONGIF_H

#include "reader/DummyYAMLConfig.h"
#include "reader/OSMPYAMLConfig.h"
#include "reader/CarlaYAMLConfig.h"
#include "reader/SUMOYAMLConfig.h"

//Generic Config
struct InterfaceYAMLConfig {
	std::string simulator;
	std::string ip;
	int port;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;
	std::vector<FMIParameter> parameter;
};

//YAML-cpp designed according to yaml-cpp tutorial: https://github.com/jbeder/yaml-cpp/wiki/Tutorial

#endif //!STANDARDYAMLCONGIF_H
