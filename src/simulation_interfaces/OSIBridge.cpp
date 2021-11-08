#include "simulation_interfaces/OSIBridge.h"

int OSIBridge::init(float starttime) {
	return 0;
}

int OSIBridge::readConfiguration(configVariants_t configVariants) {
	return 0;
}

int OSIBridge::disconnect() {
	return 0;
}

int OSIBridge::writeToInternalState() {
	for (auto address : writeAddressInformation) {
		writeToInternalState(address.second);
	}
	return 0;
}

int OSIBridge::writeToInternalState(address address)
{
	switch (getMessageType(address.name)) {
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
		readFromInternalState(address.second);
	}
	return 0;
}

int OSIBridge::readFromInternalState(address& address) {
	if ((void*)address.addr.address != nullptr) {
		//free the allocated storage of previous osimessage
		free((void*)address.addr.address);
	}
	std::string message = std::get<std::string>(mapper->mapFromInternalState(address.name, STRINGCOSIMA));
	switch (getMessageType(address.name)) {
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

eOSIMessage OSIBridge::getMessageType(std::string messageType) {
	if (messageType.find("SensorView") != std::string::npos
		&& messageType.find("Config") == std::string::npos) {
		return SensorViewMessage;
	}
	else if (messageType.find("SensorView") != std::string::npos
		&& messageType.find("Config") != std::string::npos) {
		return SensorViewConfigurationMessage;
	}
	else if (messageType.find("SensorData") != std::string::npos) { return SensorDataMessage; }
	else if (messageType.find("GroundTruth") != std::string::npos) { return GroundTruthMessage; }
	else if (messageType.find("TrafficCommand") != std::string::npos) { return TrafficCommandMessage; }
	else if (messageType.find("TrafficUpdate") != std::string::npos) { return TrafficUpdateMessage; }
	else if (messageType.find("MotionCommand") != std::string::npos) { return SL45MotionCommandMessage; }
	else if (messageType.find("VehicleCommunicationData") != std::string::npos) { return SL45VehicleCommunicationDataMessage; }
	else {
		std::cout << "Error: Can not find message " << messageType << std::endl;
		throw 5372;
	}
}
