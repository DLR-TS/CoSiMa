/**
@authors German Aerospace Center: Björn Bahn
*/

#include "catch2/catch.hpp"
#include "CoSiMa.h"

TEST_CASE("Parse runtime parameters") {
	SECTION("No runtime parameters") {
		char* argv[] = { (char*)"test.yml" };
		CmdParameter runtimeParameter = parseRuntimeParameter(1, argv);
		REQUIRE(runtimeParameter.verbose == false);
		REQUIRE(runtimeParameter.configurationPath == "");
	}
	SECTION("Only configuration path") {
		char* argv[] = { (char*)"CoSimulationManager.exe", (char*)"test.yml" };
		CmdParameter runtimeParameter = parseRuntimeParameter(2, argv);
		REQUIRE(runtimeParameter.verbose == false);
		REQUIRE(runtimeParameter.configurationPath == "test.yml");
	}
	SECTION("Verbose run") {
		char* argv[] = { (char*)"CoSimulationManager.exe", (char*)"-v" };
		CmdParameter runtimeParameter = parseRuntimeParameter(2, argv);
		REQUIRE(runtimeParameter.verbose == true);
		REQUIRE(runtimeParameter.configurationPath == "");

		char* argv2[] = { (char*)"CoSimulationManager.exe", (char*)"-d" };
		runtimeParameter = parseRuntimeParameter(2, argv2);
		REQUIRE(runtimeParameter.verbose == true);
		REQUIRE(runtimeParameter.configurationPath == "");
	}
	SECTION("Verbose run with configuration path") {
		char* argv[] = { (char*)"CoSimulationManager.exe", (char*)"test.yml", (char*)"-v" };
		CmdParameter runtimeParameter = parseRuntimeParameter(3, argv);
		REQUIRE(runtimeParameter.verbose == true);
		REQUIRE(runtimeParameter.configurationPath == "test.yml");
	}
}
