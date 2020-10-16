#include "simulation_interfaces/OSMPInterface.h"
/*
int OSMPInterface::readConfiguration(configVariants_t variant) {
	OSMPInterfaceConfig* config = std::get_if<OSMPInterfaceConfig>(&variant);
	if (nullptr == config) {
		std::cerr << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	this->config = *config;

	return 0;
}

int OSMPInterface::initialise() {
	std::ostringstream sstr;
	sstr << config.client_host << ':' << config.client_port;
	channel = grpc::CreateChannel(sstr.str(), grpc::InsecureChannelCredentials());
	stub = CoSiMa::rpc::SimulationInterface::NewStub(channel);

	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	CoSiMa::rpc::Config rpcConfig;
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

	/*if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return response.value();
	return -1;
}

std::string OSMPInterface::getStringValue(std::string base_name) {
	return "";
}

int OSMPInterface::setStringValue(std::string base_name, std::string value) {
	return 0;
}*/
