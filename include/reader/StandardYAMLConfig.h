/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef STANDARDYAMLCONGIF_H
#define STANDARDYAMLCONGIF_H
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <cctype>

enum SensorType {
	CAMERA,
	RADAR,
	LIDAR,
	ULTRASONIC,
	GENERIC
};

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
	std::string baseName;
	//Parent name is set to vehcile name or world
	std::string parentName;
	SensorType sensorType = SensorType::GENERIC;
	OSIMountingPosition sensorMountingPosition;
	//Camera, Radar, Lidar
	double field_of_view_horizontal;
	//Radar, Lidar
	double field_of_view_vertical;
	//Camera
	uint32_t number_of_pixels_horizontal;
	//Camera
	uint32_t number_of_pixels_vertical;
	//Radar, Lidar
	double emitter_frequency;
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

/**
 * YAML-cpp converter for the above defined structs. Designed according to yaml-cpp tutorial: https://github.com/jbeder/yaml-cpp/wiki/Tutorial
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
				std::cout << "Model read as null. Will be interpreted as emptpy string. "
					<<" Remove line model: in the configuration file to remove this hint."
					<< std::endl;
				osiInterface.model = "";
			}
			osiInterface.autostart = node["autostart"].IsDefined() || !node["port"].IsDefined();
			osiInterface.client_host = node["host"].IsDefined() ? node["host"].as<std::string>() : "localhost";
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
			config.baseName = nodeOrDefault<std::string>(node["base_name"]);
			//default empty string leads to configured name in additional parameters flag of Carla-OSI-Service
			config.parentName = nodeOrDefault<std::string>(node["parent_name"]);
			std::string typeName = nodeOrDefault<std::string>(node["sensor_type"]);
			//compare lower case 
			std::transform(typeName.begin(), typeName.end(), typeName.begin(),
				[](unsigned char c) { return std::tolower(c); });
			if (typeName == "camera") {
				config.sensorType = SensorType::CAMERA;
			}
			else if (typeName == "lidar") {
				config.sensorType = SensorType::LIDAR;
			}
			else if (typeName == "radar") {
				config.sensorType = SensorType::RADAR;
			}
			else if (typeName == "ultrasonic") {
				config.sensorType = SensorType::ULTRASONIC;
			}
			else if (typeName == "generic") {
				config.sensorType = SensorType::GENERIC;
			}
			else {
				std::cout << "Error parsing sensor_type: " << typeName << std::endl;
				exit(0);
			}
			config.sensorMountingPosition = nodeOrDefault<OSIMountingPosition>(node["sensor_mounting_position"]);
			config.field_of_view_horizontal = nodeOrDefault<double>(node["field_of_view_horizontal"]);
			config.field_of_view_vertical = nodeOrDefault<double>(node["field_of_view_vertical"]);
			config.number_of_pixels_horizontal = nodeOrDefault<uint32_t>(node["number_of_pixels_horizontal"]);
			config.number_of_pixels_vertical = nodeOrDefault<uint32_t>(node["number_of_pixels_vertical"]);
			config.emitter_frequency = nodeOrDefault<double>(node["emitter_frequency"]);
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
