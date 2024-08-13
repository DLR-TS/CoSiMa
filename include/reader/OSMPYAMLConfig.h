/**
@authors German Aerospace Center: Björn Bahn
*/

#pragma once

#include "reader/SharedConfig.h"

/**
* \var bool autostart
* automatically starts an OSMP Service for this model
* \var std::string model
* path to file
* \var std::vector<ConfigParameter> inputs
* holds the input osi messages
* \var std::vector<ConfigParameter> outputs
* holds the output osi messages
* \var std::vector<ConfigParameter> parameter
* parameters of the model to be set
*/
struct OSMPInterfaceConfig {
	bool autostart;
	std::string model;
	std::string client_host;
	uint16_t client_port;
	double transactionTimeout;
	double doStepTransactionTimeout;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;
	std::vector<FMIParameter> parameter;
};

namespace YAML {
	template<>
	struct convert<OSMPInterfaceConfig> {
		static Node encode(const OSMPInterfaceConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, OSMPInterfaceConfig& osmpInterface)
		{
			osmpInterface.model = node["model"].IsDefined() ? node["model"].as<std::string>() : "";
			if (osmpInterface.model == "null") {//Bugfix for entry of "model: " in configuration. This could happen easily if a recording OSMP is created.
				std::cout << "Model read as null. Will be interpreted as emptpy string. "
					<< " Remove line model: in the configuration file to remove this hint."
					<< std::endl;
				osmpInterface.model = "";
			}
			osmpInterface.autostart = node["autostart"].IsDefined() || !node["port"].IsDefined();
			osmpInterface.client_host = node["host"].IsDefined() ? node["host"].as<std::string>() : "localhost";
			osmpInterface.client_port = node["port"].IsDefined() ? node["port"].as<int>() : 0;
			osmpInterface.transactionTimeout = node["transaction_timeout"].IsDefined() ? node["transaction_timeout"].as<double>() : 0.5;
			osmpInterface.doStepTransactionTimeout = node["do_step_timeout"].IsDefined() ? node["do_step_timeout"].as<double>() : 1;
			osmpInterface.inputs = node["input"].IsDefined() ? node["input"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			osmpInterface.outputs = node["output"].IsDefined() ? node["output"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			osmpInterface.parameter = node["parameter"].IsDefined() ? node["parameter"].as<std::vector<FMIParameter>>() : std::vector<FMIParameter>();
			return true;
		}
	};
}
