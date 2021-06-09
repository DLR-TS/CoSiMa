/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "SimulationInterfaceFactory.h"

TEST_CASE("SimulationInterfaceFactory") {

	SimulationInterfaceFactory factory;
	std::shared_ptr<iSimulationData> fmiInterface = factory.makeInterface(eSimulatorName::FMI);
	REQUIRE(fmiInterface != nullptr);
	
	std::shared_ptr<iSimulationData> defaultInterface = factory.makeInterface(eSimulatorName::DEFAULT);
	REQUIRE(defaultInterface != nullptr);

	std::shared_ptr<iSimulationData> osiInterface = factory.makeInterface(eSimulatorName::OSI);
	REQUIRE(osiInterface != nullptr);

	std::shared_ptr<iSimulationData> osmpInterface = factory.makeInterface(eSimulatorName::OSMP);
	REQUIRE(osiInterface != nullptr);

	std::shared_ptr<iSimulationData> errorInterface = factory.makeInterface(eSimulatorName::SIMULATORNAME_ERROR);
	REQUIRE(errorInterface == nullptr);
}
