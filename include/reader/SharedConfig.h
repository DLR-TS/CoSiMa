/**
@authors German Aerospace Center: Björn Bahn
*/

#pragma once

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

//helper object for deciding which mapping schema an interface needs. Is used for iterating over all existing simulators in the configuration file.
struct SimulatorName {
	//name of the co-simulator
	std::string simulator;
};

struct ConfigParameter {
	//name of the variable in the interface system
	std::string interface_name;
	//name of the variable in the base system
	std::string base_name;
};

struct FMIParameter {
	//name of the parameter
	std::string name;
	//value of the parameter
	std::string value;
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
	//name of the variable in the base system
	std::string baseName;
	//Parent name is set to vehcile name or world
	std::string parentName;
	SensorType sensorType = SensorType::GENERIC;
	OSIMountingPosition sensorMountingPosition;
	//Camera, Radar, Lidar
	double field_of_view_horizontal;
	//only Radar, Lidar
	double field_of_view_vertical;
	//Only Camera
	uint32_t number_of_pixels_horizontal;
	//Only Camera
	uint32_t number_of_pixels_vertical;
	//Only Radar, Lidar
	double emitter_frequency;
};

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
}
