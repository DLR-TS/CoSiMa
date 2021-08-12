/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "configreader/YAMLConfigReader.h"
#include "base_interfaces/CARLAInterface.h"
#include "simulation_interfaces/OSIBridge.h"
#include "simulation_interfaces/OSMPBridge.h"
#include "mapper/OSIMapper.h"

TEST_CASE("Read correct Config 1", "[YAML Reader]") {
	YAMLConfigReader reader("../test/resources/testconfig1.yaml");
	SECTION("Read simulator names from config") {
		std::vector<SingleYAMLConfig> names = reader.getSimulatorNames();
		REQUIRE(names.size() == 1);
		REQUIRE(names.at(0).simulator == FMI);
		REQUIRE(names.at(0).index == 0);
	}

	SECTION("set configuration of simulator", "[YAML Reader]") {
		SingleYAMLConfig conf;
		conf.index = 0;
		conf.simulator = eSimulatorName::FMI;

		std::shared_ptr<iSimulationData> fmiSimulator = std::shared_ptr<iSimulationData>((iSimulationData*) new FMIBridge(std::shared_ptr<Mapper>((Mapper*)new FMIMapper()), false));
		fmiSimulator->getMapper()->setOwner(fmiSimulator);

		REQUIRE(reader.setConfig(fmiSimulator, conf) == 0);
	}

	SECTION("set invalid configuration of simulator", "[YAML Reader]") {
		SingleYAMLConfig conf;
		conf.index = 1;
		conf.simulator = eSimulatorName::SIMULATORNAME_ERROR;

		std::shared_ptr<iSimulationData> fmiSimulator = std::shared_ptr<iSimulationData>((iSimulationData*) new FMIBridge(std::shared_ptr<Mapper>((Mapper*)new FMIMapper()), false));

		REQUIRE(reader.setConfig(fmiSimulator, conf) == 1);
	}
}

TEST_CASE("Default prefix value for OSIMapper", "[YAML Reader]") {

	std::shared_ptr<iSimulationData> osibridge = std::shared_ptr<iSimulationData>((iSimulationData*) new OSIBridge(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()), false));
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

TEST_CASE("Read OSMP config") {
	std::shared_ptr<iSimulationData> osmpbridge = std::shared_ptr<iSimulationData>((iSimulationData*) new OSMPBridge(std::shared_ptr<Mapper>((Mapper*)new OSIMapper()), false));
	osmpbridge->getMapper()->setOwner(osmpbridge);
	SingleYAMLConfig conf;
	conf.index = 0;
	conf.simulator = eSimulatorName::OSMP;

	SECTION("Just read complete configuration") {
		YAMLConfigReader reader("../test/resources/testconfig4.yaml");
		REQUIRE(reader.setConfig(osmpbridge, conf) == 0);
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

TEST_CASE("Parse a CARLAInterfaceConfig", "[YAML Reader][CARLAInterfaceConfig]") {
	std::string config = 
R"multiline(
- simulator: CARLA
  carla_host: localhost
  carla_port: 2000 # default port
  client_host: 127.0.0.1
  client_port: 51425
  delta: 0.03
  timeout: 30 #seconds, floating point
  step_timeout: 30000 # milliseconds, integer
  initialisation_timeout: 60000 # milliseconds, integer
  sensor_view_config:
    - camera_sensor_mounting_position:
       - x: 1.4
         y: -0.2
         z: 1.7
         pitch: -9.9
         yaw: 12.5
         roll: 0.01
      prefixed_fmu_variable_name: "#prefix#OSMPSensorViewIn"
)multiline";
	auto  yaml = YAML::Load(config);

	auto carlaConfig = YAML::nodeOrDefault<CARLAInterfaceConfig>(yaml[0]);
	CHECK("localhost" == carlaConfig.carla_host);
	CHECK(2000 == carlaConfig.carla_port);
	CHECK("127.0.0.1" == carlaConfig.client_host);
	CHECK(51425 == carlaConfig.client_port);
	CHECK(Approx(0.03) == carlaConfig.deltaSeconds);
	CHECK(Approx(30.0) == carlaConfig.transactionTimeout);
	CHECK(30000 == carlaConfig.doStepTransactionTimeout);
	CHECK(60000 == carlaConfig.initialisationTransactionTimeout);
	CHECK(0 < carlaConfig.osiSensorViewConfig.size());
	CHECK("#prefix#OSMPSensorViewIn" == carlaConfig.osiSensorViewConfig[0].prefixedFmuVariableName);
	CHECK(0 < carlaConfig.osiSensorViewConfig[0].cameraSensorMountingPosition.size());
	CHECK(Approx(1.4) == carlaConfig.osiSensorViewConfig[0].cameraSensorMountingPosition[0].x);
	CHECK(Approx(-.2) == carlaConfig.osiSensorViewConfig[0].cameraSensorMountingPosition[0].y);
	CHECK(Approx(1.7) == carlaConfig.osiSensorViewConfig[0].cameraSensorMountingPosition[0].z);
	CHECK(Approx(-9.9) == carlaConfig.osiSensorViewConfig[0].cameraSensorMountingPosition[0].pitch);
	CHECK(Approx(12.5) == carlaConfig.osiSensorViewConfig[0].cameraSensorMountingPosition[0].yaw);
	CHECK(Approx(0.01) == carlaConfig.osiSensorViewConfig[0].cameraSensorMountingPosition[0].roll);
}
