#include "catch2/catch.hpp"

#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "configreader/YAMLConfigReader.h"
#include "SimulationInterfaceFactory.h"
#include "simulation_interfaces/FMIBridge.h"
#include "simulation_interfaces/OSMPBridge.h"
#include "simulation_interfaces/OSMPInterface.h"
#include "MockBaseSimulator.cpp"

std::string osimessage = "";

TEST_CASE("OSMP Test") {

	const auto osiMapper = new OSIMapper();
	auto mapper = std::shared_ptr<Mapper>((Mapper*)osiMapper);
	const auto bridge = new OSMPBridge(mapper);

	SECTION("OSMP methods")
	{
		//names from https://github.com/OpenSimulationInterface/osi-sensor-model-packaging/blob/master/examples/OSMPDummySensor/modelDescription.in.xml

		std::string name1 = "OSMPSensorViewIn.base.lo";
		std::string name2 = "OSMPSensorViewIn.base.hi";
		std::string name3 = "OSMPSensorViewIn.size";
		std::string name4 = "OSMPSensorViewInConfigRequest.base.hi";
		std::string name5 = "OSMPSensorViewInConfigRequest.base.lo";
		std::string name6 = "OSMPSensorViewInConfigRequest.size";
		std::string name7 = "OSMPSensorDataOut.base.lo";
		std::string name8 = "OSMPSensorDataOut.base.hi";
		std::string name9 = "OSMPSensorDataOut.size";
		std::string name10 = "OSMPSensorViewInConfig.base.lo";
		std::string name11 = "OSMPSensorViewInConfig.base.hi";
		std::string name12 = "OSMPSensorViewInConfig.size";

		std::string name1a = "OSMPSensorViewIn[0].base.lo";
		std::string name2a = "OSMPSensorViewIn[0].base.hi";
		std::string name3a = "OSMPSensorViewIn[0].size";
		std::string name1b = "OSMPSensorViewIn[1].base.lo";
		std::string name2b = "OSMPSensorViewIn[1].base.hi";
		std::string name3b = "OSMPSensorViewIn[1].size";

		SECTION("Fill addresses correct") {
			bridge->saveToAddressMap(bridge->inputAddresses, name1, 1234);
			bridge->saveToAddressMap(bridge->inputAddresses, name2, 5678);
			bridge->saveToAddressMap(bridge->inputAddresses, name3, 90);

			REQUIRE(bridge->inputAddresses.size() == 1);
			REQUIRE(bridge->inputAddresses.at("OSMPSensorViewIn").size == 90);
			REQUIRE(bridge->inputAddresses.at("OSMPSensorViewIn").addr.base.lo == 1234);
			REQUIRE(bridge->inputAddresses.at("OSMPSensorViewIn").addr.base.hi == 5678);

			bridge->saveToAddressMap(bridge->inputAddresses, name4, 91);
			bridge->saveToAddressMap(bridge->inputAddresses, name8, 92);
			bridge->saveToAddressMap(bridge->inputAddresses, name12, 93);

			REQUIRE(bridge->inputAddresses.size() == 4);
		}

		SECTION("Write several times the same output message with index") {
			bridge->saveToAddressMap(bridge->inputAddresses, name1a, 4321);
			bridge->saveToAddressMap(bridge->inputAddresses, name2a, 8765);
			bridge->saveToAddressMap(bridge->inputAddresses, name3a, 9090);
			bridge->saveToAddressMap(bridge->inputAddresses, name1b, 1234);
			bridge->saveToAddressMap(bridge->inputAddresses, name2b, 5678);
			bridge->saveToAddressMap(bridge->inputAddresses, name3b, 90);

			REQUIRE(bridge->inputAddresses.size() == 2);
			REQUIRE(bridge->inputAddresses.at("OSMPSensorViewIn[0]").addr.base.hi != bridge->inputAddresses.at("OSMPSensorViewIn[1]").addr.base.hi);
			REQUIRE(bridge->inputAddresses.at("OSMPSensorViewIn[0]").addr.base.lo != bridge->inputAddresses.at("OSMPSensorViewIn[1]").addr.base.lo);
			REQUIRE(bridge->inputAddresses.at("OSMPSensorViewIn[0]").size != bridge->inputAddresses.at("OSMPSensorViewIn[1]").size);
		}
	}

	SECTION("Loading of OSMP FMU") {
		std::shared_ptr<iSimulationData> simulationInterface = std::shared_ptr<iSimulationData>((iSimulationData*)bridge);
		mapper->setOwner(simulationInterface);

		SECTION("Dummy Source writes osi messages") {
			OSMPInterfaceConfig config;

			config.model = "../test/resources/OSMPDummySource.fmu";
			OSIMessageConfig message1;
			message1.base_name = "SensorView";
			message1.interface_name = "OSMPSensorViewOut";

			config.outputs.push_back(message1);

			REQUIRE(0 == mapper->readConfiguration(config));
			REQUIRE(0 == simulationInterface->init("A co-simulation fmu", 0, 0));

			REQUIRE(0 == simulationInterface->doStep());

			REQUIRE(simulationInterface->getMapper()->getInternalState()->strings.at(0).size() == 0);
			REQUIRE(0 == simulationInterface->writeToInternalState());
			REQUIRE(simulationInterface->getMapper()->getInternalState()->strings.at(0).size() != 0);

			//for later use
			osimessage = simulationInterface->getMapper()->getInternalState()->strings.at(0);
			
			REQUIRE(0 == simulationInterface->doStep());
			REQUIRE(0 == simulationInterface->writeToInternalState());
			//string looks different each timestep
			std::string newosimessage = simulationInterface->getMapper()->getInternalState()->strings.at(0);
			REQUIRE(osimessage != newosimessage);
		}

		SECTION("Dummy Sensor reads osi messages") {
			OSMPInterfaceConfig config;

			config.model = "../test/resources/OSMPDummySensor.fmu";
			OSIMessageConfig message1;
			message1.base_name = "SensorView";
			message1.interface_name = "OSMPSensorViewIn";
			config.inputs.push_back(message1);

			OSIMessageConfig message2;
			message2.base_name = "SensorData";
			message2.interface_name = "OSMPSensorDataOut";
			config.outputs.push_back(message2);

			REQUIRE(0 == mapper->readConfiguration(config));
			REQUIRE(0 == simulationInterface->init("A co-simulation fmu", 0, 0));

			REQUIRE(0 == simulationInterface->doStep());

			//no input no output
			REQUIRE(simulationInterface->getMapper()->getInternalState()->strings.at(0).size() == 0);
			REQUIRE(0 == simulationInterface->writeToInternalState());
			REQUIRE(simulationInterface->getMapper()->getInternalState()->strings.at(0).size() == 0);

			//use output of testcase before as input

			REQUIRE(osimessage.size() != 0);
			simulationInterface->getMapper()->getInternalState()->strings.at(0) = osimessage;
			REQUIRE(0 == simulationInterface->readFromInternalState());

			//do step
			REQUIRE(0 == simulationInterface->doStep());
			REQUIRE(0 == simulationInterface->writeToInternalState());
			//output has overwritten input
			REQUIRE(simulationInterface->getMapper()->getInternalState()->strings.at(0) != osimessage);
		}
	}
}

TEST_CASE("OSMP gRPC Test") {
	//Test together with OSMP-Client
	/*
	SimulationInterfaceFactory factory;
	auto osmpinterface = factory.makeInterface(OSMP);
	auto base = std::make_shared<MockBaseSimulator>();

	YAMLConfigReader reader("../test/resources/osmp-grpc-config.yaml");
	auto names = reader.getSimulatorNames();
	reader.setConfig(osmpinterface, names.at(0));

	//all inital steps
	REQUIRE(0 == osmpinterface->init("Scenario", 0.0, 0));
	REQUIRE(0 == osmpinterface->connect(""));

	//repetitive steps
	for (int i = 0; i < 5; i++) {
		REQUIRE(0 == osmpinterface->mapToInterfaceSystem(base));
		REQUIRE(0 == osmpinterface->readFromInternalState());
		REQUIRE(0 == osmpinterface->doStep());
		REQUIRE(0 == osmpinterface->writeToInternalState());
		REQUIRE(0 == osmpinterface->mapFromInterfaceSystem(base));
	}*/
}
