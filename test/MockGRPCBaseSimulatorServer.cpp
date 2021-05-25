#include "catch2/catch.hpp"

#include <future>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "grpc_proto_files/base_interface/BaseInterface.grpc.pb.h"
#include "grpc_proto_files/base_interface/BaseInterface.pb.h"
#include "grpc_proto_files/base_interface/CARLAInterface.grpc.pb.h"
#include "grpc_proto_files/base_interface/CARLAInterface.pb.h"

class MockGRPCBaseSimulatorServer : public CoSiMa::rpc::BaseInterface::Service, public CoSiMa::rpc::CARLAInterface::Service {
public:

	void RunServer(const std::string& server_address, std::promise<std::shared_ptr<grpc::Server>> promise) {
		grpc::ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(static_cast<CoSiMa::rpc::BaseInterface::Service*>(this));
		builder.RegisterService(static_cast<CoSiMa::rpc::CARLAInterface::Service*>(this));
		std::shared_ptr<grpc::Server> server(builder.BuildAndStart());
		promise.set_value(server);
		std::cout << "Server listening on " << server_address << std::endl;
		server->Wait();
		std::cout << "Server does not wait anymore" << std::endl;
		//return server;
	}

	virtual grpc::Status SetConfig(grpc::ServerContext*, const CoSiMa::rpc::CarlaConfig* config, CoSiMa::rpc::Int32* response) override {
		if (config->carla_host().size() == 0)
			response->set_value(-4);
		else if (config->carla_port() == 0)
			response->set_value(-3);
		else if (config->transaction_timeout() == 0)
			response->set_value(-2);
		else if (config->delta_seconds() < 0)
			// a value of zero indicates variable frame time and is an acceptable value
			response->set_value(-1);

		return grpc::Status::OK;
	}

	virtual grpc::Status DoStep(grpc::ServerContext*, const CoSiMa::rpc::Empty* empty, CoSiMa::rpc::Double* value) override {
		if (nullptr == value)
			value = new CoSiMa::rpc::Double();
		value->set_value(0.123456);
		return grpc::Status::OK;
	}

	virtual grpc::Status GetBoolValue(grpc::ServerContext*, const CoSiMa::rpc::String* base_name, CoSiMa::rpc::Bool* value) override {
		std::cout << "called  " << __FUNCTION__ << std::endl;
		if (0 == base_name->value().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "base_name is empty!");
		if (nullptr == value)
			value = new CoSiMa::rpc::Bool();
		value->set_value(true);
		return grpc::Status::OK;
	}

	virtual grpc::Status GetIntValue(grpc::ServerContext*, const CoSiMa::rpc::String* base_name, CoSiMa::rpc::Int32* value) override {
		std::cout << "called  " << __FUNCTION__ << std::endl;
		if (0 == base_name->value().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "base_name is empty!");
		if (nullptr == value)
			value = new CoSiMa::rpc::Int32();
		value->set_value(32);
		return grpc::Status::OK;
	}

	virtual grpc::Status GetFloatValue(grpc::ServerContext*, const CoSiMa::rpc::String* base_name, CoSiMa::rpc::Float* value) override {
		std::cout << "called  " << __FUNCTION__ << std::endl;
		if (0 == base_name->value().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "base_name is empty!");
		if (nullptr == value)
			value = new CoSiMa::rpc::Float();
		value->set_value(32.f);
		return grpc::Status::OK;
	}

	virtual grpc::Status GetDoubleValue(grpc::ServerContext*, const CoSiMa::rpc::String* base_name, CoSiMa::rpc::Double* value) override {
		std::cout << "called  " << __FUNCTION__ << std::endl;
		if (0 == base_name->value().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "base_name is empty!");
		if (nullptr == value)
			value = new CoSiMa::rpc::Double();
		value->set_value(64.0);
		return grpc::Status::OK;
	}

	virtual grpc::Status GetStringValue(grpc::ServerContext*, const CoSiMa::rpc::String* base_name, CoSiMa::rpc::Bytes* value) override {
		std::cout << "called  " << __FUNCTION__ << std::endl;
		if (0 == base_name->value().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "base_name is empty!");
		if (nullptr == value)
			value = new CoSiMa::rpc::Bytes();
		value->set_value("string");
		return grpc::Status::OK;
	}

	virtual grpc::Status SetIntValue(grpc::ServerContext* context, const CoSiMa::rpc::NamedInt32* namedValue, CoSiMa::rpc::Int32* response) override {
		if (0 == namedValue->name().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Name is empty!");
		if (nullptr == response)
			response = new CoSiMa::rpc::Int32();
		response->set_value(namedValue->value() + 1);
		return grpc::Status::OK;
	}

	virtual grpc::Status SetBoolValue(::grpc::ServerContext* context, const CoSiMa::rpc::NamedBool* namedValue, CoSiMa::rpc::Int32* response) override {
		if (0 == namedValue->name().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Name is empty!");
		if (nullptr == response)
			response = new CoSiMa::rpc::Int32();
		response->set_value(namedValue->value() ? 0 : 42);
		return grpc::Status::OK;
	}

	virtual grpc::Status SetFloatValue(::grpc::ServerContext* context, const CoSiMa::rpc::NamedFloat* namedValue, CoSiMa::rpc::Int32* response) override {
		if (0 == namedValue->name().size()) {
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Name is empty!");
		}
		if (nullptr == response)
			response = new CoSiMa::rpc::Int32();
		response->set_value(namedValue->value() + 1);
		return grpc::Status::OK;
	}

	virtual grpc::Status SetDoubleValue(::grpc::ServerContext* context, const CoSiMa::rpc::NamedDouble* namedValue, CoSiMa::rpc::Int32* response) override {
		if (0 == namedValue->name().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Name is empty!");
		if (nullptr == response)
			response = new CoSiMa::rpc::Int32();
		response->set_value(namedValue->value() + 1);
		return grpc::Status::OK;
	}

	virtual grpc::Status SetStringValue(::grpc::ServerContext* context, const CoSiMa::rpc::NamedBytes* namedValue, CoSiMa::rpc::Int32* response) override {
		if (0 == namedValue->name().size())
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Name is empty!");
		if (nullptr == response)
			response = new CoSiMa::rpc::Int32();
		response->set_value(namedValue->value().size());
		return grpc::Status::OK;
	}

};

