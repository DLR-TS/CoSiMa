/**
@authors German Aerospace Center: Bj�rn Bahn
*/

#pragma once

#include "yaml-cpp/yaml.h"

/**
* \var deltaSeconds
* simulation time delta per tick
*/
struct DummyInterfaceConfig {
	double deltaSeconds;
	double debugTimerSeconds;
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
			dummyInterface.debugTimerSeconds = node["debugTimerSeconds"].IsDefined() ? node["debugTimerSeconds"].as<double>() : 0.0;
			return true;
		}
	};
}
