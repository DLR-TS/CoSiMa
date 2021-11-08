#include "simulation_interfaces/OSMPInterface.h"

int OSMPInterface::readConfiguration(configVariants_t variant) {
	if (std::get_if<OSMPInterfaceConfig>(&variant) == nullptr) {
		std::cerr << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}
	config = std::get<OSMPInterfaceConfig>(variant);

	return 0;
}

int OSMPInterface::init(float starttime) {
	std::ostringstream sstr;
	sstr << config.client_host << ':' << config.client_port;
	grpc::ChannelArguments channelArgs;
	channelArgs.SetMaxSendMessageSize(-1);
	channelArgs.SetMaxReceiveMessageSize(-1);

	//https://grpc.github.io/grpc/core/group__grpc__arg__keys.html#ga813f94f9ac3174571dd712c96cdbbdc1
	//https://grpc.github.io/grpc/cpp/classgrpc_1_1_channel_arguments.html#ac1fa513191e8104ec57dfd6598297ce5
	channel = grpc::CreateCustomChannel(sstr.str(), grpc::InsecureChannelCredentials(), channelArgs);
	stub = CoSiMa::rpc::SimulationInterface::NewStub(channel);
	osmpStub = CoSiMa::rpc::OSMPSimulationInterface::NewStub(channel);

	CoSiMa::rpc::OSMPConfig rpcConfig;
	rpcConfig.set_fmu_path(config.model);
	for (const auto& param : config.parameter) {
		auto parameter = rpcConfig.add_parameter();
		parameter->set_name(param.name);
		parameter->set_value(param.value);
	}

	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	CoSiMa::rpc::Int32 response;

	auto status = osmpStub->SetConfig(context.get(), rpcConfig, &response);

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
		std::cerr << msg << std::endl;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return response.value();
}

int OSMPInterface::doStep(double stepsize)
{
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.doStepTransactionTimeout);

	CoSiMa::rpc::Double rpcStepSize;
	CoSiMa::rpc::Int32 rpcResponse;
	rpcStepSize.set_value(stepsize);
	if (debug) {
		std::cout << "OSMPInterface: doStep \n";
	}
	auto status = stub->DoStep(context.get(), rpcStepSize, &rpcResponse);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg << std::endl;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcResponse.value();
}

int OSMPInterface::writeToInternalState() {
	// context to handle the following rpc call
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	for (auto output : config.outputs) {
		string.set_value(output.interface_name);

		if (debug) {
			std::cout << "OSMPInterface: read " << output.interface_name;
		}
		CoSiMa::rpc::Bytes rpcValue;

		auto status = stub->GetStringValue(context.get(), string, &rpcValue);

		if (!status.ok()) {
			auto msg = status.error_message();
			std::cerr << msg << std::endl;
#ifdef __linux__
			throw std::exception();
#else
			throw std::exception(msg.c_str());
#endif
		}
		if (debug) {
			std::cout << ", Size: " << rpcValue.value().size() << "\n";
		}
		mapper->mapToInternalState(rpcValue.value(), output.interface_name, STRINGCOSIMA);
	}
	return 0;
}

int OSMPInterface::readFromInternalState() {

	for (auto input : config.inputs) {
		// context to handle the following rpc call
		std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);
		auto namedValue = CoSiMa::rpc::NamedBytes();
		namedValue.set_name(input.interface_name);
		values_t value = mapper->mapFromInternalState(input.interface_name, STRINGCOSIMA);
		namedValue.set_value(std::get<std::string>(value));

		CoSiMa::rpc::Int32 rpcRetVal;
		if (debug) {
			std::cout << "OSMPInterface: write " << input.interface_name << ", Size : " << std::get<std::string>(value).size() << std::endl;
		}
		auto status = stub->SetStringValue(context.get(), namedValue, &rpcRetVal);

		if (!status.ok()) {
			auto msg = status.error_message();
			std::cerr << msg;
#ifdef __linux__
			throw std::exception();
#else
			throw std::exception(msg.c_str());
#endif
		}
	}
	return 0;
}

int OSMPInterface::disconnect() {
	std::cout << "OSMPInterface::disconnect()" << std::endl;
	return 0;
}
