/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef OSMPINTERFACE_H
#define OSMPINTERFACE_H

#include <thread>
#include <chrono>
#include <fstream>

#include "simulation_interfaces/iSimulationData.h"
#include "mapper/OSIMapper.h"
#include "CoSiMaUtility.h"

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include "grpc_proto_files/simulation_interface/SimulationInterface.grpc.pb.h"
#include "grpc_proto_files/simulation_interface/SimulationInterface.pb.h"
#include "grpc_proto_files/simulation_interface/OSMPSimulationInterface.grpc.pb.h"
#include "grpc_proto_files/simulation_interface/OSMPSimulationInterface.pb.h"

class OSMPInterface : public SimulatorInterface {
public:
	OSMPInterface() {
		mapper = std::make_shared<OSIMapper>();
	};
	
	int init(bool verbose, std::string configurationPath) override;
	int disconnect() override;
	
	int writeToInternalState() override;
	int readFromInternalState() override;
	int doStep(double stepSize = 1) override;
	void stopSimulation() override;
	
	void configure(const YAML::Node& node) override;

	std::string getSensorViewConfigurationRequest();
	void setSensorViewConfiguration(std::string& appliedsensorViewConfiguration);
	int sensorviewindex = 0;

private:

	// grpc fields
	std::shared_ptr<grpc::Channel> channel;
	std::unique_ptr<CoSiMa::rpc::SimulationInterface::Stub> stub;
	std::unique_ptr<CoSiMa::rpc::OSMPSimulationInterface::Stub> osmpStub;
	OSMPInterfaceConfig config;

	const int retries = 5;
	int retry_counter = 0;
};

#endif //!OSMPINTERFACE_H
