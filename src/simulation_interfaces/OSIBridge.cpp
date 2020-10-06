#include "simulation_interfaces/OSIBridge.h"

int OSIBridge::init(std::string scenario, float starttime, int mode) {
	return 0;
}

int OSIBridge::readConfiguration(configVariants_t configVariants) {
	return 0;
}

int OSIBridge::connect(std::string) {
	//write osi configuration in writeAddressInformation and readAddressInformation
	return 0;
}

int OSIBridge::disconnect() {
	return 0;
}

int OSIBridge::writeToInternalState() {
	for (auto address : writeAddressInformation) {
		writeToInternalState(address.second, address.first);
	}
	return 0;
}

int OSIBridge::writeToInternalState(address address, eOSIMessage messageType)
{
	switch (messageType) {
	case SensorViewMessage:
		if(!sensorView.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(sensorView.SerializeAsString(), SensorViewMessage, address.index);
		break;
	case SensorViewConfigurationMessage:
		if(!sensorViewConfiguration.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(sensorViewConfiguration.SerializeAsString(), SensorViewConfigurationMessage, address.index);
		break;
	case SensorDataMessage:
		if(!sensorData.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(sensorData.SerializeAsString(), SensorDataMessage, address.index);
		break;
	case GroundTruthMessage:
		if(!groundTruth.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(groundTruth.SerializeAsString(), GroundTruthMessage, address.index);
		break;
	case TrafficCommandMessage:
		if(!trafficCommand.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(trafficCommand.SerializeAsString(), TrafficCommandMessage, address.index);
		break;
	case TrafficUpdateMessage:
		if(!trafficUpdate.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(trafficUpdate.SerializeAsString(), TrafficUpdateMessage, address.index);
		break;
	case SL45MotionCommandMessage:
		if(!motionCommand.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(motionCommand.SerializeAsString(), SL45MotionCommandMessage, address.index);
		break;
	case SL45VehicleCommunicationDataMessage:
		if(!vehicleCommunicationData.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(vehicleCommunicationData.SerializeAsString(), SL45VehicleCommunicationDataMessage, address.index);
		break;
	}
	return 0;
}


int OSIBridge::doStep(double stepSize) {
	return 0;
}

int OSIBridge::readFromInternalState(){
	for (auto address : readAddressInformation) {
		readFromInternalState(address.second, address.first);
	}
	return 0;
}

int OSIBridge::readFromInternalState(address& address, eOSIMessage messageType) {
	if ((void*)address.addr.address != nullptr) {
		//free the allocated storage of previous osimessage
		free((void*)address.addr.address);
	}
	switch (messageType) {
	case SensorViewMessage:
		sensorView.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SensorViewMessage, address.index));
		address.size = (int)sensorView.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorView.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorViewConfigurationMessage:
		sensorViewConfiguration.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SensorViewConfigurationMessage, address.index));
		address.size = (int)sensorViewConfiguration.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorViewConfiguration.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorDataMessage:
		sensorData.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SensorDataMessage, address.index));
		address.size = (int)sensorData.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorData.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case GroundTruthMessage:
		groundTruth.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(GroundTruthMessage, address.index));
		address.size = (int)groundTruth.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		groundTruth.SerializeToArray((void*)address.addr.address, address.size);
		break;	
	case TrafficCommandMessage:
		trafficCommand.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(TrafficCommandMessage, address.index));
		address.size = (int)trafficCommand.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		trafficCommand.SerializeToArray((void*)address.addr.address, address.size);
		break;	
	case TrafficUpdateMessage:
		trafficUpdate.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(GroundTruthMessage, address.index));
		address.size = (int)trafficUpdate.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		trafficUpdate.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SL45MotionCommandMessage:
		motionCommand.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SL45MotionCommandMessage, address.index));
		address.size = (int)motionCommand.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		motionCommand.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SL45VehicleCommunicationDataMessage:
		vehicleCommunicationData.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SL45VehicleCommunicationDataMessage, address.index));
		address.size = (int)vehicleCommunicationData.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		vehicleCommunicationData.SerializeToArray((void*)address.addr.address, address.size);
		break;
	}

	return 0;
}
