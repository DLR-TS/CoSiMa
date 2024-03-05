/**
@authors German Aerospace Center: Björn Bahn
*/

#ifndef PROXYINTERFACE_H
#define PROXYINTERFACE_H

#ifdef __linux__
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include "simulation_interfaces/iSimulationData.h"
#include "mapper/OSIMapper.h"

class ProxyInterface : public SimulatorInterface {
public:
	ProxyInterface() {
		mapper = std::make_shared<OSIMapper>();
	};

	int init(bool verbose, std::string configurationPath) override;
	int disconnect() override;

	int writeToInternalState() override;
	int readFromInternalState() override;
	int doStep(double stepSize = 1) override;
	void stopSimulation() override;

	void configure(YAML::Node& node) override;
private:
	bool verbose = false;
	OSMPInterfaceConfig config;

	osi3::SensorView sensorView;
	osi3::SensorViewConfiguration sensorViewConfiguration;
	osi3::SensorData sensorData;
	osi3::GroundTruth groundTruth;
	osi3::TrafficCommand trafficCommand;
	osi3::TrafficUpdate trafficUpdate;

	void sendToProxy(std::string& interfacename);
	void readFromProxy(std::string& interfacename);
	eOSIMessage getMessageType(const std::string& messageType);

	#ifdef __linux__
	int sockfd = 0;
	#elif _WIN32
	UINT_PTR clientSocket;
	#endif
};

#endif //!PROXYINTERFACE_H