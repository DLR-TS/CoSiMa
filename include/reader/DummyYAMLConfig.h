/**
@authors German Aerospace Center: Björn Bahn
*/

#pragma once

#include "yaml-cpp/yaml.h"

/**
* \var deltaSeconds
* simulation time delta per tick
*/
struct DummyInterfaceConfig {
	double deltaSeconds;
};

namespace YAML {
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
