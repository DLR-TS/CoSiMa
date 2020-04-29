#include "yaml-cpp/yaml.h"

struct VariableDefinitionMap {
public:
	std::string interface_name;
	std::string base_name;
	std::string type;
};

struct InterfaceYAMLConfig {
public:
	std::string ip;
	int port;
	std::vector<VariableDefinitionMap> input_map;
	std::vector<VariableDefinitionMap> output_map;
};


namespace YAML {
	template<>
	struct convert<InterfaceYAMLConfig> {
		static Node encode(const InterfaceYAMLConfig& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, InterfaceYAMLConfig& config)
		{
			config.ip = node["ip"].as<std::string>();
			config.port = node["port"].as<int>();
			config.input_map = node["input"].as<std::vector<VariableDefinitionMap>>();
			config.output_map = node["output"].as<std::vector<VariableDefinitionMap>>();
			return true;
		}
	};
}

namespace YAML {
	template<>
	struct convert<VariableDefinitionMap> {
		static Node encode(const VariableDefinitionMap& config) {
			Node node;
			return node;
		}

		static bool decode(const Node& node, VariableDefinitionMap& variableMap)
		{
			variableMap.interface_name = node["interface_name"].as<std::string>();
			variableMap.base_name = node["base_name"].as<std::string>();
			variableMap.type = node["type"].as<std::string>();
			return true;
		}
	};
}
