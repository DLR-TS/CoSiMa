#include "base_interfaces/CARLAInterface.h"

int CARLAInterface::readConfiguration(baseConfigVariants_t variant) {
	CARLAInterfaceConfig* config = std::get_if<CARLAInterfaceConfig>(&variant);
	if (nullptr == config) {
		std::cerr << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	this->config = *config;

	return 0;
}

int CARLAInterface::initialise() {
	std::ostringstream sstr;
	sstr << config.client_host << ':' << config.client_port;
	channel = grpc::CreateChannel(sstr.str(), grpc::InsecureChannelCredentials());
	stub = CoSiMa::rpc::CARLAInterface::NewStub(channel);

	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	CoSiMa::rpc::CarlaConfig rpcConfig;
	rpcConfig.set_carla_host(config.carla_host);
	rpcConfig.set_carla_port(config.carla_port);
	rpcConfig.set_transaction_timeout(config.transactionTimeout);
	rpcConfig.set_delta_seconds(config.deltaSeconds);

	CoSiMa::rpc::Int32 response;

	auto status = stub->SetConfig(&context, rpcConfig, &response);


	auto channelState = channel->GetState(true);
	switch (channelState)
	{
	case GRPC_CHANNEL_IDLE:
		std::cout << "channel is idle" << std::endl;
		return -1;
	case GRPC_CHANNEL_CONNECTING:
		std::cout << "channel is connecting" << std::endl;
		break;
	case GRPC_CHANNEL_READY:
		std::cout << "channel is ready for work" << std::endl;
		break;
	case GRPC_CHANNEL_TRANSIENT_FAILURE:
		std::cout << "channel has seen a failure but expects to recover" << std::endl;
		return -502;
	case GRPC_CHANNEL_SHUTDOWN:
		std::cout << "channel has seen a failure that it cannot recover from" << std::endl;
		return -500;
	}

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return response.value();
}

double CARLAInterface::doStep()
{
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	if (0 < config.doStepTransactionTimeout) {
		std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.doStepTransactionTimeout));
		context.set_deadline(deadline);
	}

	//TODO fix import in proto file
	//auto empty = google::protobuf::Empty();
	auto empty = CoSiMa::rpc::Empty();
	CoSiMa::rpc::Double rpcValue;

	auto status = stub->DoStep(&context, empty, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcValue.value();
}

int CARLAInterface::getIntValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Int32 rpcValue;

	auto status = stub->GetIntValue(&context, string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcValue.value();
};

bool CARLAInterface::getBoolValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Bool rpcValue;

	auto status = stub->GetBoolValue(&context, string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcValue.value();
};

float CARLAInterface::getFloatValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Float rpcValue;

	auto status = stub->GetFloatValue(&context, string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcValue.value();
};

double CARLAInterface::getDoubleValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Double rpcValue;

	auto status = stub->GetDoubleValue(&context, string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}


	return rpcValue.value();
};

std::string CARLAInterface::getStringValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::String rpcValue;

	auto status = stub->GetStringValue(&context, string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcValue.value();
};

int CARLAInterface::setIntValue(std::string base_name, int value) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;

	auto namedValue = CoSiMa::rpc::NamedInt32();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetIntValue(&context, namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcRetVal.value();
};

int CARLAInterface::setBoolValue(std::string base_name, bool value) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto namedValue = CoSiMa::rpc::NamedBool();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetBoolValue(&context, namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcRetVal.value();
};

int CARLAInterface::setFloatValue(std::string base_name, float value) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto namedValue = CoSiMa::rpc::NamedFloat();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetFloatValue(&context, namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcRetVal.value();
};

int CARLAInterface::setDoubleValue(std::string base_name, double value) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto namedValue = CoSiMa::rpc::NamedDouble();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetDoubleValue(&context, namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcRetVal.value();
};

int CARLAInterface::setStringValue(std::string base_name, std::string value) {
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	auto namedValue = CoSiMa::rpc::NamedString();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetStringValue(&context, namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcRetVal.value();
};
