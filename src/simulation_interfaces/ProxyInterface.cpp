#include "simulation_interfaces/ProxyInterface.h"

void ProxyInterface::configure(const YAML::Node& node) {
	config = node.as<OSMPInterfaceConfig>();
	configureMapperOwner();
	mapper->readConfiguration(config);
}

int ProxyInterface::init(bool verbose, std::string configurationPath) {
	this->verbose = verbose;

#ifdef __linux__
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(config.client_port);
	inet_pton(AF_INET, config.client_host.c_str(), &serv_addr.sin_addr);

	connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
#elif _WIN32
	WSADATA wsa_data;
	WSAStartup(MAKEWORD(2, 0), &wsa_data);

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(config.client_port);
	InetPton(AF_INET, config.client_host.c_str(), &serv_addr.sin_addr.s_addr);

	connect(clientSocket, reinterpret_cast<SOCKADDR *>(&serv_addr), sizeof(serv_addr));
#endif
	return 0;
}

int ProxyInterface::writeToInternalState() {
	for (auto& input : config.outputs) {
		readFromProxy(input.interface_name);
	}
	return 0;
}

int ProxyInterface::readFromInternalState() {
	for (auto output : config.inputs) {
		sendToProxy(output.interface_name);
	}
	return 0;
}

int ProxyInterface::doStep(double stepsize) {
	return 0;
}

void ProxyInterface::stopSimulation() {
}

int ProxyInterface::disconnect() {
#ifdef __linux__
	close(sockfd);
#elif _WIN32
	closesocket(clientSocket);
	WSACleanup();
#endif
	std::cout << "ProxyInterface::disconnect()" << std::endl;
	return 0;
}

void ProxyInterface::readFromProxy(std::string& interfacename) {

	int messageSize = 0;
	long readableLength = 0;
#ifdef __linux__
	while(true) {
		ioctl(sockfd, FIONREAD, &readableLength);
		if (readableLength < 4) {//4 bytes for message length
			continue;
		}
		break;
	}

	int bytesRead = read(sockfd, &messageSize, sizeof(int));
	if (messageSize != 0) {
		char** message = new char*[messageSize + 1];
		bool allReceived = false;
		int bytesReceived = 0;
		while (!allReceived) {
			ioctl(sockfd, FIONREAD, &readableLength);
			if (readableLength != 0) {
				if (readableLength > messageSize) {
					readableLength = messageSize - bytesReceived;
				}
				char messagePart[readableLength];
				bytesRead = read(sockfd, &messagePart, readableLength);

				std::memcpy((void*)(message + bytesReceived), messagePart, bytesRead);
				bytesReceived += bytesRead;
				if (bytesReceived == messageSize) {
					allReceived = true;
				}
			}
		}
#elif _WIN32
	//(server,);
	char** message = new char*[messageSize + 1];
#endif
		std::string value;
		switch (getMessageType(interfacename)) {
		case SensorViewMessage:
			if (!sensorView.ParseFromArray(message, messageSize)) {
				std::cerr << "SensorView: Parse unsuccessful" << std::endl;
			}
			if (verbose) {
				std::cout << sensorView.DebugString() << std::endl;
			}
			value = sensorView.SerializeAsString();
			break;
		case SensorViewConfigurationMessage:
			if (!sensorViewConfiguration.ParseFromArray(message, messageSize)) {
				std::cerr << "SensorViewConfiguration: Parse unsuccessful" << std::endl;
			}
			if (verbose) {
				std::cout << sensorViewConfiguration.DebugString() << std::endl;
			}
			value = sensorViewConfiguration.SerializeAsString();
			break;
		case SensorDataMessage:
			if (!sensorData.ParseFromArray(message, messageSize)) {
				std::cerr << "SensorData: Parse unsuccessful" << std::endl;
			}
			if (verbose) {
				std::cout << sensorData.DebugString() << std::endl;
			}
			value = sensorData.SerializeAsString();
			break;
		case GroundTruthMessage:
			if (!groundTruth.ParseFromArray(message, messageSize)) {
				std::cerr << "GroundTruth: Parse unsuccessful" << std::endl;
			}
			if (verbose) {
				std::cout << groundTruth.DebugString() << std::endl;
			}
			value = groundTruth.SerializeAsString();
			break;
		case TrafficCommandMessage:
			if (!trafficCommand.ParseFromArray(message, messageSize)) {
				std::cerr << "TrafficCommand: Parse unsuccessful" << std::endl;
			}
			if (verbose) {
				std::cout << trafficCommand.DebugString() << std::endl;
			}
			value = trafficCommand.SerializeAsString();
			break;
		case TrafficUpdateMessage:
			if (!trafficUpdate.ParseFromArray(message, messageSize)) {
				std::cerr << "TrafficUpdate: Parse unsuccessful" << std::endl;
			}
			if (verbose) {
				std::cout << trafficUpdate.DebugString() << std::endl;
			}
			value = trafficUpdate.SerializeAsString();
			break;
		}
		mapper->mapToInternalState(value, interfacename);

#ifdef __linux__
	}
	else {
		std::cout << "No data to read" << std::endl;
	}
#elif _WIN32

#endif
}

void ProxyInterface::sendToProxy(std::string& interfacename) {
	std::string value = mapper->mapFromInternalState(interfacename);
	address address;
	switch (getMessageType(interfacename)) {
	case SensorViewMessage:
		sensorView.ParseFromString(value);
		address.size = (int)sensorView.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		if (verbose && value.size()) {
			std::cout << sensorView.DebugString() << std::endl;
		}
		sensorView.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorViewConfigurationMessage:
		sensorViewConfiguration.ParseFromString(value);
		address.size = (int)sensorViewConfiguration.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		if (verbose && value.size()) {
			std::cout << sensorViewConfiguration.DebugString() << std::endl;
		}
		sensorViewConfiguration.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case SensorDataMessage:
		sensorData.ParseFromString(value);
		address.size = (int)sensorData.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		if (verbose && value.size()) {
			std::cout << sensorData.DebugString() << std::endl;
		}
		sensorData.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case GroundTruthMessage:
		groundTruth.ParseFromString(value);
		address.size = (int)groundTruth.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		if (verbose && value.size()) {
			std::cout << groundTruth.DebugString() << std::endl;
		}
		groundTruth.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case TrafficCommandMessage:
		trafficCommand.ParseFromString(value);
		address.size = (int)trafficCommand.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		if (verbose && value.size()) {
			std::cout << trafficCommand.DebugString() << std::endl;
		}
		trafficCommand.SerializeToArray((void*)address.addr.address, address.size);
		break;
	case TrafficUpdateMessage:
		trafficUpdate.ParseFromString(value);
		address.size = (int)trafficUpdate.ByteSizeLong();
		address.addr.address = (unsigned long long)malloc(address.size);
		if (verbose && value.size()) {
			std::cout << trafficUpdate.DebugString() << std::endl;
		}
		trafficUpdate.SerializeToArray((void*)address.addr.address, address.size);
		break;
	}
#ifdef __linux__
	send(sockfd, &address.size, sizeof(address.size), 0);
	send(sockfd, (void*)address.addr.address, address.size, 0);
#elif _WIN32
	send(clientSocket, (char*)&address.size, sizeof(address.size), 0);
	send(clientSocket, (char*)&address.addr.address, address.size, 0);
#endif
	free((void*)address.addr.address);
}

eOSIMessage ProxyInterface::getMessageType(const std::string& messageType) {
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
	else {
		std::cout << "Error: Can not find message " << messageType << std::endl;
		throw - 1;
	}
}

bool ProxyInterface::isAutostart(uint16_t& port) {
	return false; 
}

void ProxyInterface::setPort(uint16_t port) {
	config.client_port = port;
}
