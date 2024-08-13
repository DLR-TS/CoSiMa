/**
@authors German Aerospace Center: Bj�rn Bahn
*/

#pragma once
#include "yaml-cpp/yaml.h"
#include "reader/SharedConfig.h"

/**
* \var client_host
* host name or ip of the CARLA to OSI client
* \var client_port
* port of the CARLA to OSI client
* \var carla_host
* host name or ip of the CARLA server to which the client should connect
* \var carla_port
* host port of the CARLA server to which the client should connect
* \var autostart
* start the Base System Service by CoSiMa
* \var transactionTimeout
* transaction timeout in seconds
* \var deltaSeconds
* simulation time delta per tick
* \var initializationTransactionTimeout
* timout for initial transaction
* \var doStepTransactionTimeout
* maximum amount of time in seconds allowed for step calculation and gRPC transaction. Unlimited if set to zero
* \var additionalParameter
* additional parameters for configuration of Carla OSI Service
* \var osiSensorViewConfig
* sensorview configs
*/
struct CARLAInterfaceConfig {
	std::string client_host;
	uint16_t client_port;
	std::string carla_host;
	uint16_t carla_port;
	bool autostart;
	double transactionTimeout;
	double deltaSeconds;
	uint32_t initializationTransactionTimeout;
	uint32_t doStepTransactionTimeout;
	double debugTimerSeconds;
	std::string additionalParameters;
	std::vector<SensorViewConfig> osiSensorViewConfig;
};

namespace YAML {
	template<>
	struct convert<CARLAInterfaceConfig> {
		static Node encode(const CARLAInterfaceConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, CARLAInterfaceConfig& carlaInterface)
		{
			carlaInterface.carla_host = node["carla_host"].IsDefined() ? node["carla_host"].as<std::string>() : "localhost";
			carlaInterface.carla_port = node["carla_port"].IsDefined() ? node["carla_port"].as<int>() : 2000;
			carlaInterface.client_host = node["client_host"].IsDefined() ? node["client_host"].as<std::string>() : "localhost";
			carlaInterface.client_port = node["client_port"].IsDefined() ? node["client_port"].as<int>() : 51425;
			carlaInterface.autostart = node["autostart"].IsDefined() ? node["autostart"].as<bool>() : false;
			carlaInterface.deltaSeconds = node["delta"].IsDefined() ? node["delta"].as<double>() : 0;
			carlaInterface.transactionTimeout = node["transaction_timeout"].IsDefined() ? node["transaction_timeout"].as<double>() : 0;
			carlaInterface.doStepTransactionTimeout = node["do_step_timeout"].IsDefined() ? node["do_step_timeout"].as<uint32_t>() : 0;
			carlaInterface.initializationTransactionTimeout = node["initialisation_timeout"].IsDefined() ? node["initialisation_timeout"].as<uint32_t>() :
				node["initialization_timeout"].IsDefined() ? node["initialization_timeout"].as<uint32_t>() : 60000; //1 minute timeout

			carlaInterface.osiSensorViewConfig = nodeOrDefault<std::vector<SensorViewConfig>>(node["sensor_view_config"]);
			carlaInterface.additionalParameters = node["additional_parameters"].IsDefined() ? node["additional_parameters"].as<std::string>() : "";
			carlaInterface.debugTimerSeconds = node["debug_timer_seconds"].IsDefined() ? node["debug_timer_seconds"].as<double>() : 0.0;
			return true;
		}
	};
}
