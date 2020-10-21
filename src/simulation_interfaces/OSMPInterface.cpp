#include "simulation_interfaces/OSMPInterface.h"

int OSMPInterface::readConfiguration(configVariants_t variant) {
	OSMPInterfaceConfig* config = std::get_if<OSMPInterfaceConfig>(&variant);
	if (nullptr == config) {
		std::cerr << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	this->config = *config;

	return 0;
}

int OSMPInterface::init(std::string scenario, float starttime, int mode) {
	std::ostringstream sstr;
	sstr << config.client_host << ':' << config.client_port;
	channel = grpc::CreateChannel(sstr.str(), grpc::InsecureChannelCredentials());
	stub = CoSiMa_rpc::SimulationInterface::NewStub(channel);

	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);
	
	CoSiMa_rpc::SimConfig rpcConfig;
	rpcConfig.set_fmu(config.model);

	CoSiMa_rpc::SimInt32 response;

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

int OSMPInterface::connect(std::string) {
	return 0;
}

int OSMPInterface::doStep(double stepsize)
{
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	if (0 < config.doStepTransactionTimeout) {
		std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.doStepTransactionTimeout));
		context.set_deadline(deadline);
	}

	//TODO fix import in proto file
	//auto empty = google::protobuf::Empty();
	auto empty = CoSiMa_rpc::SimEmpty();
	CoSiMa_rpc::SimDouble rpcValue;

	auto status = stub->DoStep(&context, empty, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		throw new std::exception(msg.c_str());
	}

	return rpcValue.value();
}
int OSMPInterface::writeToInternalState() {
	// context to handle the following rpc call
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa_rpc::SimString();
	for (auto output : config.outputs) {
		string.set_value(output.interface_name);

		CoSiMa_rpc::SimString rpcValue;

		auto status = stub->GetStringValue(context.get(), string, &rpcValue);

		if (!status.ok()) {
			auto msg = status.error_message();
			throw new std::exception(msg.c_str());
		}
		mapper->mapToInternalState(rpcValue.value(), output.interface_name, STRINGCOSIMA);
	}
	return 0;
}

int OSMPInterface::readFromInternalState() {

	// context to handle the following rpc call
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	for (auto input : config.inputs) {
		auto namedValue = CoSiMa_rpc::SimNamedString();
		namedValue.set_name(input.interface_name);
		values_t value = mapper->mapFromInternalState(input.interface_name, STRINGCOSIMA);
		namedValue.set_value(std::get<std::string>(value));

		CoSiMa_rpc::SimInt32 rpcRetVal;

		auto status = stub->SetStringValue(context.get(), namedValue, &rpcRetVal);

		if (!status.ok()) {
			auto msg = status.error_message();
			throw new std::exception(msg.c_str());
		}
	}
	return 0;
}

int OSMPInterface::disconnect() {
	return 0;
}
