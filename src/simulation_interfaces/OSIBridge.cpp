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
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(sensorView.SerializeAsString(), SensorViewMessage);
		break;
	case SensorViewConfigurationMessage:
		if(!sensorViewConfiguration.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(sensorViewConfiguration.SerializeAsString(), SensorViewConfigurationMessage);
		break;
	case SensorDataMessage:
		if(!sensorData.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(sensorData.SerializeAsString(), SensorDataMessage);
		break;
	case GroundTruthMessage:
		if(!groundTruth.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(groundTruth.SerializeAsString(), SensorViewConfigurationMessage);
		break;
	case SL45MotionCommandMessage:
		if(!motionCommand.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(motionCommand.SerializeAsString(), SL45MotionCommandMessage);
		break;
	case SL45VehicleCommunicationDataMessage:
		if(!vehicleCommunicationData.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		std::static_pointer_cast<OSIMapper>(mapper)->mapOSIToInternalState(vehicleCommunicationData.SerializeAsString(), SL45VehicleCommunicationDataMessage);
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
	free((void*)address.addr.address);
	switch (messageType) {
	case SensorViewMessage:
		sensorView.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SensorViewMessage));
		address.size = (int)sensorView.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorView.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorViewConfigurationMessage:
		sensorViewConfiguration.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SensorViewConfigurationMessage));
		address.size = (int)sensorViewConfiguration.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorViewConfiguration.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorDataMessage:
		sensorData.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SensorDataMessage));
		address.size = (int)sensorData.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorData.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case GroundTruthMessage:
		groundTruth.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(GroundTruthMessage));
		address.size = (int)groundTruth.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		groundTruth.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SL45MotionCommandMessage:
		motionCommand.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SL45MotionCommandMessage));
		address.size = (int)motionCommand.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		motionCommand.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SL45VehicleCommunicationDataMessage:
		vehicleCommunicationData.ParseFromString(std::static_pointer_cast<OSIMapper>(mapper)->mapOSIFromInternalState(SL45VehicleCommunicationDataMessage));
		address.size = (int)vehicleCommunicationData.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		vehicleCommunicationData.SerializeToArray((void*)address.addr.address, address.size);
		break;
	}

	return 0;
}
