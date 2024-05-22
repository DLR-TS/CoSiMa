/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "reader/YAMLConfigReader.h"
#include "base_interfaces/CARLAInterface.h"
#include "mapper/OSIMapper.h"

TEST_CASE("Load yaml configuration file") {
	SECTION("No config") {
		REQUIRE(loadConfigurationFile("").IsNull() == true);
	}

	SECTION("Config does not exist") {
		REQUIRE(loadConfigurationFile("../test/resources/testconfig-does-not-exist.yaml").IsNull() == true);
	}

	SECTION("Config does exist") {
		REQUIRE(loadConfigurationFile("../test/resources/testconfig-dummy.yaml").IsNull() == false);
	}
}

TEST_CASE("Get simulator type from name") {
	SECTION("dummy") {
		REQUIRE(nameToEnum("DUMMY") == eSimulatorTypes::DUMMY);
	}

	SECTION("generic") {
		REQUIRE(nameToEnum("generic") == eSimulatorTypes::DUMMY);
	}

	SECTION("carla - lowercase") {
		REQUIRE(nameToEnum("carla") == eSimulatorTypes::CARLA);
	}

	SECTION("carla - uppercase") {
		REQUIRE(nameToEnum("CARLA") == eSimulatorTypes::CARLA);
	}

	SECTION("OSMP") {
		REQUIRE(nameToEnum("OSMP") == eSimulatorTypes::OSMP);
	}

	SECTION("not a name") {
		REQUIRE(nameToEnum("Not a valid simulator") == eSimulatorTypes::SIMULATORNAME_ERROR);
	}
}

TEST_CASE("Get simulator types from configuration") {
	SECTION("Read osmp simulator names from config") {
		YAML::Node node = loadConfigurationFile("../test/resources/testconfig-osmp.yaml");
		SimulationSetup config = parseSimulationConfiguration(node);
		REQUIRE(config.childSimulators.size() == 1);
		REQUIRE(!config.baseSimulator);
		REQUIRE(config.valid == false);//no base simulator
	}

	SECTION("Read carla simulator names from config") {
		YAML::Node node = loadConfigurationFile("../test/resources/testConfig-carla.yaml");
		SimulationSetup config = parseSimulationConfiguration(node);
		REQUIRE(config.baseSimulator);
		REQUIRE(config.childSimulators.size() == 0);
		REQUIRE(config.valid == true);
	}

	SECTION("Read dummy simulator names from config") {
		YAML::Node node = loadConfigurationFile("../test/resources/testConfig-dummy.yaml");
		SimulationSetup config = parseSimulationConfiguration(node);
		REQUIRE(config.baseSimulator);
		REQUIRE(config.childSimulators.size() == 0);
		REQUIRE(config.valid == true);
	}

	SECTION("Read wrong simulator names from config") {
		YAML::Node node = loadConfigurationFile("../test/resources/testConfig-wrong.yaml");
		SimulationSetup config = parseSimulationConfiguration(node);
		REQUIRE(!config.baseSimulator);
		REQUIRE(config.childSimulators.size() == 0);
		REQUIRE(config.valid == false);
	}

	SECTION("Read osmp simulator names from config") {
		YAML::Node node = loadConfigurationFile("../test/resources/testconfig-combined.yaml");
		SimulationSetup config = parseSimulationConfiguration(node);
		REQUIRE(config.baseSimulator);
		REQUIRE(config.childSimulators.size() == 2);
		REQUIRE(config.valid == true);
	}
}
