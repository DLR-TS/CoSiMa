/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef STANDARDYAMLCONGIF_H
#define STANDARDYAMLCONGIF_H
#include "yaml-cpp/yaml.h"
#include <iostream>

/**
 * \var std::string simulator
 * helper object for deciding which mapping schema an interface needs. Is used for iterating over all existing simulators in the configuration file.
 */
struct SimulatorName {
	std::string simulator;
};

struct OSIMountingPosition {
	double x;
	double y;
	double z;
	double pitch;
	double yaw;
	double roll;
};

struct SensorViewConfig {
	std::string prefixedFmuVariableName;
	std::vector<OSIMountingPosition> genericSensorMountingPosition;
	std::vector<OSIMountingPosition> radarSensorMountingPosition;
	std::vector<OSIMountingPosition> lidarSensorMountingPosition;
	std::vector<OSIMountingPosition> cameraSensorMountingPosition;
	std::vector<OSIMountingPosition> ultrasonicSensorMountingPosition;
};

/**
* \var deltaSeconds
* simulation time delta per tick
*/
struct DummyInterfaceConfig {
	double deltaSeconds;
};

/**
* \var client_host
* host name or ip of the CARLA to OSI client
* \var client_port
* port of the CARLA to OSI client
* \var carla_host
* host name or ip of the CARLA server to which the client should connect
* \var carla_port
* host port of the CARLA server to which the client should connect
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
	double transactionTimeout;
	double deltaSeconds;
	uint32_t initializationTransactionTimeout;
	uint32_t doStepTransactionTimeout;
	std::string additionalParameters;
	std::vector<SensorViewConfig> osiSensorViewConfig;
};

/**
* \var std::string interface_name
* holds name of the variable in the interface system
* \var std::string base_name
* holds name of the variable in the base system
*/
struct ConfigParameter {
	std::string interface_name;
	std::string base_name;
};

/**
* \var std::string name
* holds name of the parameter
* \var std::string value
* holds value of the variable
*/
struct FMIParameter {
	std::string name;
	std::string value;
};

/**
* \var std::string interface_name
* holds name of the variable in the interface system
* \var std::string base_name
* holds name of the variable in the base system
* \var std::string type
* holds type name of the variable
*/

struct InterfaceYAMLConfig {
	std::string simulator;
	std::string ip;
	int port;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;
	std::vector<FMIParameter> parameter;
};

/**
* \var std::string model
* path to FMU (file) //TODO should later point to Specification of System Structure and Parameterization (*.ssp file)
* \var std::vector<ConfigParameter> inputs
* holds the input osi messages
* \var std::vector<ConfigParameter> outputs
* holds the output osi messages
*/
struct OSMPInterfaceConfig {
	std::string model;
	std::string client_host;
	uint16_t client_port;
	double transactionTimeout;
	double doStepTransactionTimeout;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;
	std::vector<FMIParameter> parameter;
};

/**
 * YAML-cpp converter for the above defined structs. Designed according to yaml-cpp tutorial: https://github.com/jbeder/yaml-cpp/wiki/Tutorial

 *
 */

namespace YAML {
	template<typename T>
	static T nodeOrDefault(const Node& node, T defaultValue) {
		return node.IsDefined() ? node.as<T>() : defaultValue;
	};
	template<typename T>
	static T nodeOrDefault(const Node& node) {
		return node.IsDefined() ? node.as<T>() : T();
	};

	template<>
	struct convert<SimulatorName> {
		static Node encode(const SimulatorName& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, SimulatorName& config)
		{
			config.simulator = node["simulator"].as<std::string>();
			return true;
		}
	};

	template<>
	struct convert<InterfaceYAMLConfig> {
		static Node encode(const InterfaceYAMLConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, InterfaceYAMLConfig& config)
		{
			config.simulator = node["simulator"].as<std::string>();
			config.ip = node["ip"].IsDefined() ? node["ip"].as<std::string>() : std::string();
			if (node["port"].IsDefined()) {
				config.port = node["port"].as<int>();
			}
			else {
				std::cout << "No port for " << config.simulator << " is set. Default value 0 is used. May not be a problem since not every interface needs a defined port.";
				config.port = 0;
			}
			config.inputs = node["input"].IsDefined() ? node["input"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			config.outputs = node["output"].IsDefined() ? node["output"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			config.parameter = node["parameter"].IsDefined() ? node["parameter"].as<std::vector<FMIParameter>>() : std::vector<FMIParameter>();
			return true;
		}
	};

	template<>
	struct convert<FMIParameter> {
		static Node encode(const FMIParameter& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, FMIParameter& fmiParameter)
		{
			fmiParameter.name = node["name"].as<std::string>();
			fmiParameter.value = node["value"].as<std::string>();
			return true;
		}
	};

	template<>
	struct convert<ConfigParameter> {
		static Node encode(const ConfigParameter& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, ConfigParameter& osiMessage)
		{
			osiMessage.interface_name = node["interface_name"].as<std::string>();
			osiMessage.base_name = node["base_name"].as<std::string>();
			return true;
		}
	};

	template<>
	struct convert<OSMPInterfaceConfig> {
		static Node encode(const OSMPInterfaceConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, OSMPInterfaceConfig& osiInterface)
		{
			osiInterface.model = node["model"].IsDefined() ? node["model"].as<std::string>() : "";
			if (osiInterface.model == "null") {//Bugfix for entry of "model: " in configuration. This could happen easily if a recording OSMP is created.
				std::cout << "Model read as null. Will be interpreted as emptpy string. Remove line model: in the configuration file to remove this hint."
					std::endl;
				osiInterface.model = "";
			}
			osiInterface.client_host = node["host"].IsDefined() ? node["host"].as<std::string>() : "";
			osiInterface.client_port = node["port"].IsDefined() ? node["port"].as<int>() : 0;
			osiInterface.transactionTimeout = node["transaction_timeout"].IsDefined() ? node["transaction_timeout"].as<double>() : 0.5;
			osiInterface.doStepTransactionTimeout = node["do_step_timeout"].IsDefined() ? node["do_step_timeout"].as<double>() : 1;
			osiInterface.inputs = node["input"].IsDefined() ? node["input"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			osiInterface.outputs = node["output"].IsDefined() ? node["output"].as<std::vector<ConfigParameter>>() : std::vector<ConfigParameter>();
			osiInterface.parameter = node["parameter"].IsDefined() ? node["parameter"].as<std::vector<FMIParameter>>() : std::vector<FMIParameter>();
			return true;
		}
	};

	template<>
	struct convert<OSIMountingPosition> {
		static Node encode(const OSIMountingPosition config) {
			return Node();
		}

		static bool decode(const Node& node, OSIMountingPosition& config) {
			config.x = nodeOrDefault<double>(node["x"]);
			config.y = nodeOrDefault<double>(node["y"]);
			config.z = nodeOrDefault<double>(node["z"]);
			config.pitch = nodeOrDefault<double>(node["pitch"]);
			config.yaw = nodeOrDefault<double>(node["yaw"]);
			config.roll = nodeOrDefault<double>(node["roll"]);
			return true;
		}
	};

	template<>
	struct convert<SensorViewConfig> {
		static Node encode(const SensorViewConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, SensorViewConfig& config)
		{
			config.prefixedFmuVariableName = nodeOrDefault<std::string>(node["prefixed_fmu_variable_name"]);
			config.cameraSensorMountingPosition = nodeOrDefault<std::vector<OSIMountingPosition>>(node["camera_sensor_mounting_position"]);
			config.radarSensorMountingPosition = nodeOrDefault<std::vector<OSIMountingPosition>>(node["radar_sensor_mounting_position"]);
			config.lidarSensorMountingPosition = nodeOrDefault<std::vector<OSIMountingPosition>>(node["lidar_sensor_mounting_position"]);
			config.genericSensorMountingPosition = nodeOrDefault<std::vector<OSIMountingPosition>>(node["generic_sensor_mounting_position"]);
			config.ultrasonicSensorMountingPosition = nodeOrDefault<std::vector<OSIMountingPosition>>(node["ultrasonic_sensor_mounting_position"]);
			return true;
		}
	};

	template<>
	struct convert<CARLAInterfaceConfig> {
		static Node encode(const CARLAInterfaceConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, CARLAInterfaceConfig& carlaInterface)
		{
			carlaInterface.carla_host = node["carla_host"].IsDefined() ? node["carla_host"].as<std::string>() : "";
			carlaInterface.carla_port = node["carla_port"].IsDefined() ? node["carla_port"].as<int>() : 0;
			carlaInterface.client_host = node["client_host"].IsDefined() ? node["client_host"].as<std::string>() : "";
			carlaInterface.client_port = node["client_port"].IsDefined() ? node["client_port"].as<int>() : 0;
			carlaInterface.deltaSeconds = node["delta"].IsDefined() ? node["delta"].as<double>() : 0;
			carlaInterface.transactionTimeout = node["transaction_timeout"].IsDefined() ? node["transaction_timeout"].as<double>() : 0;
			carlaInterface.doStepTransactionTimeout = node["do_step_timeout"].IsDefined() ? node["do_step_timeout"].as<uint32_t>() : 0;
			carlaInterface.initializationTransactionTimeout = node["initialisation_timeout"].IsDefined() ? node["initialisation_timeout"].as<uint32_t>() :
				node["initialization_timeout"].IsDefined() ? node["initialization_timeout"].as<uint32_t>() : 60000; //1 minute timeout

			carlaInterface.osiSensorViewConfig = nodeOrDefault<std::vector<SensorViewConfig>>(node["sensor_view_config"]);
			carlaInterface.additionalParameters = node["additional_parameters"].IsDefined() ? node["additional_parameters"].as<std::string>() : "";
			return true;
		}
	};

	template<>
	struct convert<DummyInterfaceConfig> {
		static Node encode(const DummyInterfaceConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, DummyInterfaceConfig& dummyInterface)
		{
			dummyInterface.deltaSeconds = node["delta"].IsDefined() ? node["delta"].as<double>() : 0;
			return true;
		}
	};
}

#endif //!STANDARDYAMLCONGIF_H
