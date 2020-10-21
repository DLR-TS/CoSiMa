#ifndef OSMPINTERFACE_H
#define OSMPINTERFACE_H

#include "simulation_interfaces/iSimulationData.h"
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include "grpc_proto_files/simulation_interface/SimulationInterface.grpc.pb.h"
#include "grpc_proto_files/simulation_interface/SimulationInterface.pb.h"

class OSMPInterface/* : public iSimulationData */{
/*
	int readConfiguration(configVariants_t config);
	int initialise();

	int setStringValue(std::string base_name, std::string value);
	std::string getStringValue(std::string base_name);
	
private:

	// grpc fields
	std::shared_ptr<grpc::Channel> channel;
	std::unique_ptr<CoSiMa::rpc::SimulationInterface::Stub> stub;
	OSMPInterfaceConfig config;
	*/
};

#endif //!OSMPINTERFACE_H