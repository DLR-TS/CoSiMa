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
	grpc::ChannelArguments channelArgs;
	channelArgs.SetMaxSendMessageSize(-1);
	channelArgs.SetMaxReceiveMessageSize(-1);
	std::cout << channelArgs.c_channel_args().num_args << std::endl;
	//channelArgs.SetInt("grpc.max_receive_message_length", -1);
	//channelArgs.SetInt("grpc.max_send_message_length", -1);

	//channelArgs.SetString("grpc.max_receive_message_length", "-1"); <- message: must be an integer
	//channelArgs.SetString("grpc.max_send_message_length", "-1"); <- message: must be an integer
	std::cout << channelArgs.c_channel_args().num_args << std::endl;
	//std::cout << channelArgs.c_channel_args().num_args << std::endl;
	//std::cout << channelArgs.c_channel_args().num_args << std::endl;

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

	grpc::ClientContext context;
	std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.transactionTimeout*1e3));
	context.set_deadline(deadline);

	CoSiMa::rpc::Int32 response;
	
	auto status = osmpStub->SetConfig(&context, rpcConfig, &response);

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

int OSMPInterface::connect(std::string) {
	return 0;
}

int OSMPInterface::doStep(double stepsize)
{
	//std::cout << "DoStep" << std::endl;
	// context to handle the following rpc call - cannot be reused
	grpc::ClientContext context;
	if (0 < config.doStepTransactionTimeout) {
		std::chrono::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds((uint64_t)(config.doStepTransactionTimeout));
		context.set_deadline(deadline);
	}

	CoSiMa::rpc::Double rpcStepSize;
	CoSiMa::rpc::Int32 rpcResponse;
	rpcStepSize.set_value(stepsize);

	auto status = stub->DoStep(&context, rpcStepSize, &rpcResponse);

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
	//std::cout << "Write" << std::endl;
	// context to handle the following rpc call
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	for (auto output : config.outputs) {
		string.set_value(output.interface_name);

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
		mapper->mapToInternalState(rpcValue.value(), output.interface_name, STRINGCOSIMA);
	}
	return 0;
}

int OSMPInterface::readFromInternalState() {
	//std::cout << "Read" << std::endl;

	for (auto input : config.inputs) {
		// context to handle the following rpc call
		std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);
		auto namedValue = CoSiMa::rpc::NamedBytes();
		namedValue.set_name(input.interface_name);
		values_t value = mapper->mapFromInternalState(input.interface_name, STRINGCOSIMA);
		namedValue.set_value(std::get<std::string>(value));

		CoSiMa::rpc::Int32 rpcRetVal;
		if (std::get<std::string>(value).size() < 100) {
			std::cout << input.interface_name << " : " << std::get<std::string>(value) << std::endl;
		}
		else {
			std::cout << input.interface_name << " : Large OSI Message Size : " << std::get<std::string>(value).size() << std::endl;
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
