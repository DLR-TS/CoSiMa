#ifndef CARLAINTERFACE_H
#define CARLAINTERFACE_H

#include <string>

#include "base_interfaces/BaseSystemInterface.h"
#include "configreader/BaseConfigVariants.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "grpc_proto_files/base_interface/CARLAInterface.grpc.pb.h"
#include "grpc_proto_files/base_interface/CARLAInterface.pb.h"

class CARLAInterface : public BaseSystemInterface
{
	CARLAInterfaceConfig config;

	// grpc fields
	std::shared_ptr<grpc::Channel> channel;
	std::unique_ptr<CoSiMa::rpc::CARLAInterface::Stub> stub;

public:
	virtual int readConfiguration(baseConfigVariants_t config) override;
	virtual int initialise() override;
	virtual double doStep() override;

	virtual int getIntValue(std::string base_name) override;
	virtual bool getBoolValue(std::string base_name) override;
	virtual float getFloatValue(std::string base_name) override;
	virtual double getDoubleValue(std::string base_name) override;
	virtual std::string getStringValue(std::string base_name) override;

	virtual int setIntValue(std::string base_name, int value) override;
	virtual int setBoolValue(std::string base_name, bool value) override;
	virtual int setFloatValue(std::string base_name, float value) override;
	virtual int setDoubleValue(std::string base_name, double value) override;
	virtual int setStringValue(std::string base_name, std::string value) override;
};

#endif // !CARLAINTERFACE_H