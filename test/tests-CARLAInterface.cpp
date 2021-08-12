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

	REQUIRE(0 == carla->initialise(false, false));

	CHECK(!carla->getBoolValue("some_name"));
	CHECK(0 == carla->getIntValue("some_name"));
	CHECK(0.f == carla->getFloatValue("some_name"));
	CHECK(0 == carla->getDoubleValue("some_name"));
	CHECK("string" == carla->getStringValue("some_name"));

	CHECK(0 == carla->setBoolValue("some_name", true));
	CHECK(0 == carla->setBoolValue("some_name", false));
	CHECK(0 == carla->setIntValue("some_name", -1));
	CHECK(0 == carla->setIntValue("some_name", 0));
	CHECK(0.f == carla->setFloatValue("some_name", -1.f));
	CHECK(0.f == carla->setFloatValue("some_name", 0.f));
	CHECK(0.0 == carla->setDoubleValue("some_name", -1.0));
	CHECK(0.0 == carla->setDoubleValue("some_name", 0.0));

	CHECK(0.123456 == carla->doStep());

	server->Shutdown();
	t.join();
}
