/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef CARLAINTERFACE_H
#define CARLAINTERFACE_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

#include "BaseSystemInterface.h"
#include "reader/BaseConfigVariants.h"
#include "CoSiMaUtility.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "grpc_proto_files/base_interface/BaseInterface.grpc.pb.h"
#include "grpc_proto_files/base_interface/BaseInterface.pb.h"
#include "grpc_proto_files/base_interface/CARLAInterface.grpc.pb.h"
#include "grpc_proto_files/base_interface/CARLAInterface.pb.h"

class CARLAInterface : public BaseSystemInterface
{
	CARLAInterfaceConfig config;

	// grpc fields
	std::shared_ptr<grpc::Channel> channel;
	std::unique_ptr<CoSiMa::rpc::BaseInterface::Stub> stub;
	std::unique_ptr<CoSiMa::rpc::CARLAInterface::Stub> configStub;

public:
	virtual void configure(YAML::Node node) override;
	virtual int init(bool debug) override;
	virtual double doStep(double stepSize = 1) override;
	virtual double getStepSize() override;
	virtual int disconnect() override;

	virtual std::string getOSIMessage(const std::string& base_name) override;

	virtual int setOSIMessage(const std::string& base_name, const std::string& value) override;

private:
	CoSiMa::rpc::CarlaConfig parseConfigToGRPC();
	void copyMountingPositions(const SensorViewConfig& sensorViewConfig, osi3::MountingPosition* rpcMountingPosition);
};

#endif // !CARLAINTERFACE_H
