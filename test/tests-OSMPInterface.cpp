#include "catch2/catch.hpp"

#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "simulation_interfaces/FMIBridge.h"
#include "simulation_interfaces/OSMPBridge.h"

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

		SECTION("Interpret the OSMPNames correct") {

			REQUIRE(bridge->getMessageType(name1) == eOSIMessage::SensorViewMessage);
			REQUIRE(bridge->getMessageType(name2) == eOSIMessage::SensorViewMessage);
			REQUIRE(bridge->getMessageType(name3) == eOSIMessage::SensorViewMessage);
			REQUIRE(bridge->getMessageType(name4) == eOSIMessage::SensorViewConfigurationMessage);
			REQUIRE(bridge->getMessageType(name5) == eOSIMessage::SensorViewConfigurationMessage);
			REQUIRE(bridge->getMessageType(name6) == eOSIMessage::SensorViewConfigurationMessage);
			REQUIRE(bridge->getMessageType(name7) == eOSIMessage::SensorDataMessage);
			REQUIRE(bridge->getMessageType(name8) == eOSIMessage::SensorDataMessage);
			REQUIRE(bridge->getMessageType(name9) == eOSIMessage::SensorDataMessage);
			REQUIRE(bridge->getMessageType(name10) == eOSIMessage::SensorViewConfigurationMessage);
			REQUIRE(bridge->getMessageType(name11) == eOSIMessage::SensorViewConfigurationMessage);
			REQUIRE(bridge->getMessageType(name12) == eOSIMessage::SensorViewConfigurationMessage);
		}

		SECTION("Fill addresses correct") {
			bridge->saveToAddressMap(name1, 1234);
			bridge->saveToAddressMap(name2, 5678);
			bridge->saveToAddressMap(name3, 90);

			REQUIRE(bridge->addresses.size() == 1);
			REQUIRE(bridge->addresses.at("OSMPSensorViewIn").size == 90);
			REQUIRE(bridge->addresses.at("OSMPSensorViewIn").addr.base.lo == 1234);
			REQUIRE(bridge->addresses.at("OSMPSensorViewIn").addr.base.hi == 5678);

			bridge->saveToAddressMap(name4, 91);
			bridge->saveToAddressMap(name8, 92);
			bridge->saveToAddressMap(name12, 93);

			REQUIRE(bridge->addresses.size() == 4);
		}
	}

	SECTION("Loading of OSMP FMU") {
		std::shared_ptr<iSimulationData> simulationInterface = std::shared_ptr<iSimulationData>((iSimulationData*)bridge);
		mapper->setOwner(simulationInterface);

		SECTION("Dummy Source") {
			OSMPInterfaceConfig config;

			config.model = "../test/resources/OSMPDummySource.fmu";
			OSIMessageConfig message1;
			message1.base_name = "SensorView";
			message1.interface_name = "SensorView";

			config.outputs.push_back(message1);

			REQUIRE(0 == mapper->readConfiguration(config));
			REQUIRE(0 == simulationInterface->init("A co-simulation fmu", 0, 0));

			REQUIRE(0 == simulationInterface->doStep());

			REQUIRE(simulationInterface->getMapper()->getInternalState()->strings.at(0).size() == 0);
			REQUIRE(0 == simulationInterface->writeToInternalState());
			REQUIRE(simulationInterface->getMapper()->getInternalState()->strings.at(0).size() != 0);
			osimessage = simulationInterface->getMapper()->getInternalState()->strings.at(0);
		}

		SECTION("Dummy Sensor") {
			OSMPInterfaceConfig config;

			config.model = "../test/resources/OSMPDummySensor.fmu";
			OSIMessageConfig message1;
			message1.base_name = "SensorView";
			message1.interface_name = "SensorView";
			config.inputs.push_back(message1);

			OSIMessageConfig message2;
			message2.base_name = "SensorData";
			message2.interface_name = "SensorData";
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
