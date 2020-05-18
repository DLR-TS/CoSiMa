#include "catch2/catch.hpp"

#include "simulation_interfaces/iSimulationData.h"
#include "simulation_interfaces/FMIBridge.h"
#include "mapper/FMIMapper.h"

TEST_CASE("internalstate is availible") {
	std::shared_ptr<Mapper> mapper = std::shared_ptr<Mapper>((Mapper*)new FMIMapper());
	iSimulationData* simulationdata = (iSimulationData*) new FMIBridge(mapper);

	simulationdata->getInternalState()->bools.push_back(true);
	simulationdata->getInternalState()->bools.push_back(false);
	REQUIRE(simulationdata->getInternalState()->bools.at(0) == true);
	REQUIRE(simulationdata->getInternalState()->bools.at(1) == false);
	simulationdata->getInternalState()->doubles.push_back(0.0);
	simulationdata->getInternalState()->doubles.push_back(1000.5);
	REQUIRE(simulationdata->getInternalState()->doubles.at(0) == 0.0);
	REQUIRE(simulationdata->getInternalState()->doubles.at(1) == 1000.5);
	simulationdata->getInternalState()->floats.push_back(0.67f);
	simulationdata->getInternalState()->floats.push_back(-7.9f);
	REQUIRE(simulationdata->getInternalState()->floats.at(0) == 0.67f);
	REQUIRE(simulationdata->getInternalState()->floats.at(1) == -7.9f);
	simulationdata->getInternalState()->integers.push_back(10056);
	simulationdata->getInternalState()->integers.push_back(-59574);
	REQUIRE(simulationdata->getInternalState()->integers.at(0) == 10056);
	REQUIRE(simulationdata->getInternalState()->integers.at(1) == -59574);
	simulationdata->getInternalState()->strings.push_back("base_name");
	simulationdata->getInternalState()->strings.push_back("test1");
	REQUIRE(simulationdata->getInternalState()->strings.at(0) == "base_name");
	REQUIRE(simulationdata->getInternalState()->strings.at(1) == "test1");
}

TEST_CASE("set mapper") {
	std::shared_ptr<Mapper> mapper = std::shared_ptr<Mapper>((Mapper*)new FMIMapper());
	iSimulationData* simulationdata = (iSimulationData*) new FMIBridge(mapper);

	REQUIRE(simulationdata->getMapper() == mapper);
}
