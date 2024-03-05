/**
@authors German Aerospace Center: Björn Bahn
*/

#include "catch2/catch.hpp"
#include "CoSiMa.h"

TEST_CASE("Parse runtime parameters") {
	SECTION("No runtime parameters") {
		char* argv[] = { (char*)"test.yml" };
		Cosima c;
		CmdParameter runtimeParameter = c.parseRuntimeParameter(1, argv);
		REQUIRE(runtimeParameter.verbose == false);
		REQUIRE(runtimeParameter.configurationPath == "");
	}
	SECTION("Only configuration path") {
		char* argv[] = { (char*)"CoSimulationManager.exe", (char*)"directory_10/test.yml" };
		Cosima c;
		CmdParameter runtimeParameter = c.parseRuntimeParameter(2, argv);
		REQUIRE(runtimeParameter.verbose == false);
		REQUIRE(runtimeParameter.configurationPath == "directory_10");
		REQUIRE(runtimeParameter.configurationName == "test.yml");
	}
	SECTION("Verbose run") {
		char* argv[] = { (char*)"CoSimulationManager.exe", (char*)"-v" };
		Cosima c;
		CmdParameter runtimeParameter = c.parseRuntimeParameter(2, argv);
		REQUIRE(runtimeParameter.verbose == true);
		REQUIRE(runtimeParameter.configurationPath == "");

		char* argv2[] = { (char*)"CoSimulationManager.exe", (char*)"-d" };
		runtimeParameter = c.parseRuntimeParameter(2, argv2);
		REQUIRE(runtimeParameter.verbose == true);
		REQUIRE(runtimeParameter.configurationPath == "");
	}
	SECTION("Verbose run with configuration path") {
		char* argv[] = { (char*)"CoSimulationManager.exe", (char*)"test.yml", (char*)"-v" };
		Cosima c;
		CmdParameter runtimeParameter = c.parseRuntimeParameter(3, argv);
		REQUIRE(runtimeParameter.verbose == true);
		REQUIRE(runtimeParameter.configurationPath == "");
		REQUIRE(runtimeParameter.configurationName == "test.yml");
	}
}
