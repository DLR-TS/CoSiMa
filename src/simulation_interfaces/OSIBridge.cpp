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
		mapper->mapToInternalState(sensorView.SerializeAsString(), address.name, STRINGCOSIMA);
		break;
	case SensorViewConfigurationMessage:
		if(!sensorViewConfiguration.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		mapper->mapToInternalState(sensorViewConfiguration.SerializeAsString(), address.name, STRINGCOSIMA);
		break;
	case SensorDataMessage:
		if(!sensorData.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		mapper->mapToInternalState(sensorData.SerializeAsString(), address.name, STRINGCOSIMA);
		break;
	case GroundTruthMessage:
		if(!groundTruth.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		mapper->mapToInternalState(groundTruth.SerializeAsString(), address.name, STRINGCOSIMA);
		break;
	case TrafficCommandMessage:
		if(!trafficCommand.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		mapper->mapToInternalState(trafficCommand.SerializeAsString(), address.name, STRINGCOSIMA);
		break;
	case TrafficUpdateMessage:
		if(!trafficUpdate.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		mapper->mapToInternalState(trafficUpdate.SerializeAsString(), address.name, STRINGCOSIMA);
		break;
	case SL45MotionCommandMessage:
		if(!motionCommand.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		mapper->mapToInternalState(motionCommand.SerializeAsString(), address.name, STRINGCOSIMA);
		break;
	case SL45VehicleCommunicationDataMessage:
		if(!vehicleCommunicationData.ParseFromArray((const void*)address.addr.address, address.size)){
			return 1;
		}
		mapper->mapToInternalState(vehicleCommunicationData.SerializeAsString(), address.name, STRINGCOSIMA);
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
	std::string message = std::get<std::string>(mapper->mapFromInternalState(address.name, STRINGCOSIMA));
	switch (messageType) {
	case SensorViewMessage:
		sensorView.ParseFromString(message);
		address.size = (int)sensorView.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorView.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorViewConfigurationMessage:
		sensorViewConfiguration.ParseFromString(message);
		address.size = (int)sensorViewConfiguration.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorViewConfiguration.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorDataMessage:
		sensorData.ParseFromString(message);
		address.size = (int)sensorData.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		sensorData.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case GroundTruthMessage:
		groundTruth.ParseFromString(message);
		address.size = (int)groundTruth.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		groundTruth.SerializeToArray((void*)address.addr.address, address.size);
		break;	
	case TrafficCommandMessage:
		trafficCommand.ParseFromString(message);
		address.size = (int)trafficCommand.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		trafficCommand.SerializeToArray((void*)address.addr.address, address.size);
		break;	
	case TrafficUpdateMessage:
		trafficUpdate.ParseFromString(message);
		address.size = (int)trafficUpdate.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		trafficUpdate.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SL45MotionCommandMessage:
		motionCommand.ParseFromString(message);
		address.size = (int)motionCommand.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		motionCommand.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SL45VehicleCommunicationDataMessage:
		vehicleCommunicationData.ParseFromString(message);
		address.size = (int)vehicleCommunicationData.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		vehicleCommunicationData.SerializeToArray((void*)address.addr.address, address.size);
		break;
	}

	return 0;
}
