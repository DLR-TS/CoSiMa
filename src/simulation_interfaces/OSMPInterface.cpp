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

	if (verbose) {
		std::cout << "Try to connect to " << config.client_host << ":" << config.client_port << std::endl;
	}
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

	for (const auto& param : config.parameter) {
		auto parameter = rpcConfig.add_parameter();
		parameter->set_name(param.name);
		parameter->set_value(param.value);
	}

	//send FMU before rest of configuration
	if (!sendFMU()) {
		//if FMU could not be send the path may be a local path in OMSP environment
		rpcConfig.set_fmupath(config.model);
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
      		retry_counter++;
		if (retry_counter < retries) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "Retry" << std::endl;
			return init(starttime);
		}
		return -502;
	case GRPC_CHANNEL_SHUTDOWN:
		std::cout << "channel has seen a failure that it cannot recover from" << std::endl;
		return -500;
	}

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg << std::endl;
		throw std::exception();
	}
	return response.value();
}

bool OSMPInterface::sendFMU() {

	std::ifstream is(config.model, std::ios::binary);
	if (is) {
		// get length of file:
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);

		// allocate memory:
		char* buffer = new char[length];

		// read data as a block:
		is.read(buffer, length);

		is.close();

		CoSiMa::rpc::FMU fmu;
		fmu.set_binaryfmu(buffer, length);

		// context to handle the following rpc call - cannot be reused
		std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

		CoSiMa::rpc::UploadStatus response;

		auto status = osmpStub->UploadFMU(context.get(), fmu, &response);

		if (response.code() != CoSiMa::rpc::UploadStatusCode::Ok) {
			std::cout << "Could not send given FMU path: " << config.model
				<< " Will try this path as a local path direct in OSMP environment." << std::endl;
			return false;
		}
	} else {
		std::cout << "Could not find given FMU path: " << config.model
			<< " Will try this path as a local path direct in OSMP environment." << std::endl;
		return false;
	}

	return true;
}

int OSMPInterface::doStep(double stepsize)
{
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.doStepTransactionTimeout);

	CoSiMa::rpc::Double rpcStepSize;
	CoSiMa::rpc::Int32 rpcResponse;
	rpcStepSize.set_value(stepsize);
	if (verbose) {
		std::cout << "OSMPInterface: doStep \n";
	}
	auto status = stub->DoStep(context.get(), rpcStepSize, &rpcResponse);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg << std::endl;
		throw std::exception();
	}

	return rpcResponse.value();
}

int OSMPInterface::writeToInternalState() {

	auto string = CoSiMa::rpc::String();
	for (auto output : config.outputs) {
		//context to handle the following rpc call
		std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);
		string.set_value(output.interface_name);

		CoSiMa::rpc::Bytes rpcValue;

		auto status = stub->GetStringValue(context.get(), string, &rpcValue);

		if (!status.ok()) {
			auto msg = status.error_message();
			std::cerr << msg << std::endl;
			throw std::exception();
		}
		if (verbose) {
			std::cout << "OSMPInterface: read " << output.interface_name << ", Size: " << rpcValue.value().size()
				<< ", Hash: " << std::hash<std::string>{}(rpcValue.value()) << std::endl;
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
		if (verbose) {
			std::cout << "OSMPInterface: write " << input.interface_name << ", Size: " << std::get<std::string>(value).size()
				<< ", Hash: " << std::hash<std::string>{}(std::get<std::string>(value)) << std::endl;
		}
		auto status = stub->SetStringValue(context.get(), namedValue, &rpcRetVal);

		if (!status.ok()) {
			auto msg = status.error_message();
			std::cerr << msg << std::endl;
			throw std::exception();
		}
	}
	return 0;
}

int OSMPInterface::disconnect() {
	std::cout << "OSMPInterface::disconnect()" << std::endl;
	return 0;
}
