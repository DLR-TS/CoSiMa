/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "simulation_interfaces/OSIBridge.h"
#include "osi_sensorview.pb.h"
#include "osi_common.pb.h"
#include "configreader/StandardYAMLConfig.h"

TEST_CASE("OSIBridge Test") {

	const auto osiMapper = new OSIMapper();
	auto mapper = std::shared_ptr<Mapper>((Mapper*)osiMapper);
	OSIBridge bridge(mapper);

	SECTION("write to internalstate") {
	
		int hostId = 1234;

		osi3::SensorView sensorView;
		osi3::Identifier* hostid = new osi3::Identifier();
		hostid->set_value(hostId);
		sensorView.set_allocated_host_vehicle_id(hostid);
		long byteSize = sensorView.ByteSizeLong();
		
		//Array and pointer
		void* data[5];
		bool b = sensorView.SerializeToArray(data, byteSize);
		REQUIRE(b == true);
		address address;
		address.addr.address = (unsigned long long)data;
		address.size = byteSize;
		address.name = "SensorView";

		//YAML Config
		OSIInterfaceConfig config;
		config.prefix = "#";
		OSIMessageConfig varDef;
		varDef.base_name = "SensorViewBaseName";
		varDef.interface_name = "SensorView";
		config.outputs.push_back(varDef);

		bridge.getMapper()->readConfiguration(config);

		int returnValue = bridge.writeToInternalState(address);
		
		REQUIRE(returnValue == 0);
		REQUIRE(bridge.getMapper()->getInternalState()->strings.at(0).size() == 5);

		//check if message is correct in internal state
		osi3::SensorView sensorView2;
		sensorView2.ParseFromString(bridge.getMapper()->getInternalState()->strings.at(0));
		REQUIRE(hostId == sensorView2.mutable_host_vehicle_id()->value());
	}

	SECTION("read from internalstate") {
	
		int hostId = 5678;

		osi3::SensorView sensorView;
		osi3::Identifier* hostid = new osi3::Identifier();
		hostid->set_value(hostId);
		sensorView.set_allocated_host_vehicle_id(hostid);
		int byteSize = sensorView.ByteSize();
		
		//Array and pointer
		void* data = malloc(byteSize);
		address address;
		address.addr.address = (unsigned long long)data;
		address.size = byteSize;
		address.name = "SensorView";

		//YAML Config
		OSIInterfaceConfig config;
		OSIMessageConfig varDef;
		varDef.base_name = "SensorViewBaseName";
		varDef.interface_name = "SensorView";
		config.inputs.push_back(varDef);

		bridge.getMapper()->readConfiguration(config);
		//write to internalState
		bridge.getMapper()->getInternalState()->strings.at(0) = sensorView.SerializeAsString();
		
		REQUIRE(bridge.readFromInternalState(address) == 0);

		osi3::SensorView sensorView2;
		sensorView2.ParseFromArray((const void*)address.addr.address, 5);
		REQUIRE(sensorView2.mutable_host_vehicle_id()->value() == hostId);
	}

	SECTION("Interpret the OSMPNames correct") {

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

		REQUIRE(bridge.getMessageType(name1) == eOSIMessage::SensorViewMessage);
		REQUIRE(bridge.getMessageType(name2) == eOSIMessage::SensorViewMessage);
		REQUIRE(bridge.getMessageType(name3) == eOSIMessage::SensorViewMessage);
		REQUIRE(bridge.getMessageType(name4) == eOSIMessage::SensorViewConfigurationMessage);
		REQUIRE(bridge.getMessageType(name5) == eOSIMessage::SensorViewConfigurationMessage);
		REQUIRE(bridge.getMessageType(name6) == eOSIMessage::SensorViewConfigurationMessage);
		REQUIRE(bridge.getMessageType(name7) == eOSIMessage::SensorDataMessage);
		REQUIRE(bridge.getMessageType(name8) == eOSIMessage::SensorDataMessage);
		REQUIRE(bridge.getMessageType(name9) == eOSIMessage::SensorDataMessage);
		REQUIRE(bridge.getMessageType(name10) == eOSIMessage::SensorViewConfigurationMessage);
		REQUIRE(bridge.getMessageType(name11) == eOSIMessage::SensorViewConfigurationMessage);
		REQUIRE(bridge.getMessageType(name12) == eOSIMessage::SensorViewConfigurationMessage);
	}
}
