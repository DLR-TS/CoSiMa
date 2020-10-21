#ifndef OSMPINTERFACE_H
#define OSMPINTERFACE_H

#include "simulation_interfaces/iSimulationData.h"

#include "CoSiMaUtility.h"

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include "grpc_proto_files/simulation_interface/SimulationInterface.grpc.pb.h"
#include "grpc_proto_files/simulation_interface/SimulationInterface.pb.h"

class OSMPInterface : public iSimulationData {
	OSMPInterface(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};
	
	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;
	
	int writeToInternalState() override;
	int readFromInternalState() override;
	int doStep(double stepSize = 1) override;
	
	int readConfiguration(configVariants_t configVariants) override;

private:

	// grpc fields
	std::shared_ptr<grpc::Channel> channel;
	std::unique_ptr<CoSiMa_rpc::SimulationInterface::Stub> stub;
	OSMPInterfaceConfig config;

};

#endif //!OSMPINTERFACE_H