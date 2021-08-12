/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include <thread>

#include "base_interfaces/CARLAInterface.h"
#include "configreader/StandardYAMLConfig.h"

#include "MockGRPCBaseSimulatorServer.cpp"

TEST_CASE("CARLAInterface", "[CARLAInterface]") {
	// host, port, transaction timeout, delta seconds (1/framerate)
	CARLAInterfaceConfig config{ "localhost",  51425, "localhost", 2000u, 5, (1.0 / 60) };

	std::shared_ptr<CARLAInterface> carla = std::make_shared<CARLAInterface>();
	REQUIRE(0 == carla->readConfiguration(config));

	MockGRPCBaseSimulatorServer mockGRPCBaseSimulatorServer;
	std::promise< std::shared_ptr<grpc::Server>> promise;
	auto future = promise.get_future();
	//mockGRPCBaseSimulatorServer.RunServer("localhost:51424");
	//auto t = std::thread([&mockGRPCBaseSimulatorServer, &promise]() {mockGRPCBaseSimulatorServer.RunServer("localhost:51425", std::move(promise)); });
	auto t = std::thread(&MockGRPCBaseSimulatorServer::RunServer, &mockGRPCBaseSimulatorServer, "localhost:51425", std::move(promise));

	auto server = future.get();

	REQUIRE(0 == carla->initialise(false));

	CHECK(carla->getBoolValue("some_name"));
	CHECK_THROWS(carla->getBoolValue(""));
	CHECK(32 == carla->getIntValue("some_name"));
	CHECK_THROWS(carla->getIntValue(""));
	CHECK(32.f == carla->getFloatValue("some_name"));
	CHECK_THROWS(carla->getFloatValue(""));
	CHECK(64.0 == carla->getDoubleValue("some_name"));
	CHECK_THROWS(carla->getDoubleValue(""));
	CHECK("string" == carla->getStringValue("some_name"));
	CHECK_THROWS(carla->getStringValue(""));

	CHECK(0 == carla->setBoolValue("some_name", true));
	CHECK(42 == carla->setBoolValue("some_name", false));
	CHECK_THROWS(carla->setBoolValue("", true));
	CHECK(0 == carla->setIntValue("some_name", -1));
	CHECK(1 == carla->setIntValue("some_name", 0));
	CHECK_THROWS(carla->setIntValue("", 1));
	CHECK(0.f == carla->setFloatValue("some_name", -1.f));
	CHECK(1.f == carla->setFloatValue("some_name", 0.f));
	CHECK_THROWS(carla->setFloatValue("", 1.f));
	CHECK(0.0 == carla->setDoubleValue("some_name", -1.0));
	CHECK(1.0 == carla->setDoubleValue("some_name", 0.0));
	CHECK_THROWS(carla->setDoubleValue("", 1.0));

	CHECK(0.123456 == carla->doStep());

	server->Shutdown();
	t.join();
}
