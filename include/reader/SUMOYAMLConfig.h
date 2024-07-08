/**
@authors German Aerospace Center: Björn Bahn
*/

#pragma once

#include "reader/SharedConfig.h"

/**
* \var bool autostart
* automatically starts an OSMP Service for this model
* \var std::string model
* path to FMU (file)
* \var std::vector<ConfigParameter> inputs
* holds the input osi messages
* \var std::vector<ConfigParameter> outputs
* holds the output osi messages
* \var std::vector<ConfigParameter> parameter
* parameters of the model to be set
*/
struct SUMOInterfaceConfig {
	std::string model;
	double transactionTimeout;
	double doStepTransactionTimeout;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;
};

namespace YAML {
	template<>
	struct convert<SUMOInterfaceConfig> {
		static Node encode(const SUMOInterfaceConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, SUMOInterfaceConfig& sumoInterface)
		{
			sumoInterface.model = node["model"].IsDefined() ? node["model"].as<std::string>() : "";
			if (sumoInterface.model == "null") {//Bugfix for entry of "model: " in configuration. This could happen easily if a recording OSMP is created.
				std::cout << "Model read as null. Will be interpreted as test.sumocfg"
					<< " Remove line model: in the configuration file to remove this hint."
					<< std::endl;
				sumoInterface.model = "test.sumocfg";
			}
			sumoInterface.transactionTimeout = node["transaction_timeout"].IsDefined() ? node["transaction_timeout"].as<double>() : 0.5;
			sumoInterface.doStepTransactionTimeout = node["do_step_timeout"].IsDefined() ? node["do_step_timeout"].as<double>() : 1;
			sumoInterface.inputs = node["input"].IsDefined() ? node["input"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			sumoInterface.outputs = node["output"].IsDefined() ? node["output"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			return true;
		}
	};
}
