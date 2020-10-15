#include "catch2/catch.hpp"

#include "configreader/YAMLConfigReader.h"
#include "base_interfaces/CARLAInterface.h"
#include "simulation_interfaces/VTDBridge.h"
#include "simulation_interfaces/OSIBridge.h"
#include "mapper/VTDMapper.h"
#include "mapper/OSIMapper.h"

TEST_CASE("Read correct Config 1", "[YAML Reader]") {
	YAMLConfigReader reader("../test/resources/testconfig1.yaml");
	SECTION("Read simulator names from config") {
		std::vector<SingleYAMLConfig> names = reader.getSimulatorNames();
		REQUIRE(names.size() == 3);
		REQUIRE(names.at(0).simulator == VTD);
		REQUIRE(names.at(0).index == 0);
		REQUIRE(names.at(1).simulator == FMI);
		REQUIRE(names.at(1).index == 0);
		REQUIRE(names.at(2).simulator == VTD);
		REQUIRE(names.at(2).index == 1);
	}

	SECTION("set configuration of simulator", "[YAML Reader]") {
		SingleYAMLConfig conf;
		conf.index = 1;
		conf.simulator = eSimulatorName::VTD;

		std::shared_ptr<iSimulationData> vtdSimulator = std::shared_ptr<iSimulationData>((iSimulationData*) new VTDBridge(std::shared_ptr<Mapper>((Mapper*)new VTDMapper())));
		vtdSimulator->getMapper()->setOwner(vtdSimulator);

		REQUIRE(reader.setConfig(vtdSimulator, conf) == 0);
	}

	SECTION("set invalid configuration of simulator", "[YAML Reader]") {
		SingleYAMLConfig conf;
		conf.index = 1;
		conf.simulator = eSimulatorName::SIMULATORNAME_ERROR;

		std::shared_ptr<iSimulationData> vtdSimulator = std::shared_ptr<iSimulationData>((iSimulationData*) new VTDBridge(std::shared_ptr<Mapper>((Mapper*)new VTDMapper())));

		REQUIRE(reader.setConfig(vtdSimulator, conf) == 1);
	}
}

TEST_CASE("Default prefix value for OSIMapper", "[YAML Reader]") {

	std::shared_ptr<iSimulationData> osibridge = std::shared_ptr<iSimulationData>((iSimulationData*) new OSIBridge(std::shared_ptr<Mapper>((Mapper*)new OSIMapper())));
	osibridge->getMapper()->setOwner(osibridge);
	SingleYAMLConfig conf;
	conf.index = 0;
	conf.simulator = eSimulatorName::OSI;

	SECTION("Prefix is set"){
		YAMLConfigReader reader("../test/resources/testconfig2.yaml");
		reader.setConfig(osibridge, conf);
		REQUIRE(std::static_pointer_cast<OSIMapper>(osibridge->getMapper())->prefix == "#prefix#");
	}

	SECTION("Prefix is not set (default value used)") {
		YAMLConfigReader reader("../test/resources/testconfig3.yaml");
		reader.setConfig(osibridge, conf);
		REQUIRE(std::static_pointer_cast<OSIMapper>(osibridge->getMapper())->prefix == "#");
	}
}

TEST_CASE("ReadCarla config from file", "[YAML Reader]") {
	YAMLConfigReader reader("../test/resources/carlaConfig.yaml");
	std::vector<SingleYAMLConfig> names = reader.getSimulatorNames();

	REQUIRE(names[0].simulator == CARLA);

	std::shared_ptr<BaseSystemInterface> baseSystem = std::make_shared<CARLAInterface>();
	int success = reader.setBaseSystemConfig(baseSystem, names[0]);
	REQUIRE(success == 0);
}
