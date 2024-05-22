#include "simulation_interfaces/OSMPInterface.h"

void OSMPInterface::configure(const YAML::Node& node) {
	config = node.as<OSMPInterfaceConfig>();
	configureMapperOwner();
	mapper->readConfiguration(config);
}

int OSMPInterface::init(bool verbose, std::string configurationPath) {
	this->verbose = verbose;
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
	if (config.model == "") {
		std::cout << "OSMP Service will run as a recoder since no file is given as input." << std::endl;
	} else {
		std::ifstream is(config.model, std::ios::binary);
		if (!is) {//check if model is located next to configuration file
			is = std::ifstream(configurationPath + "/" + config.model, std::ios::binary);
		}
		if (is) {
			// get length of file:
			is.seekg(0, is.end);
			auto length = is.tellg();
			is.seekg(0, is.beg);

			// allocate memory:
			char* buffer = new char[length];

			// read data as a block:
			is.read(buffer, length);
			is.close();
			rpcConfig.set_binaryfile(buffer, length);
			delete[] buffer;
		} else {
			std::cout << "Could not find given file path: " << config.model
				<< " Will try this path as a local path direct in OSMP environment." << std::endl;
		}
	}
	//if data could not be send the path may be a local path in OMSP environment
	rpcConfig.set_filepath(config.model);

	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	CoSiMa::rpc::Status response;

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
			return init(verbose, configurationPath);
		}
		return -502;
	case GRPC_CHANNEL_SHUTDOWN:
		std::cout << "channel has seen a failure that it cannot recover from" << std::endl;
		return -500;
	}

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg << std::endl;
		return 1;
	}
	return 0;
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
			std::cout << "End of simulation" << std::endl;
			return 1;
		}
		if (verbose) {
			std::cout << "OSMPInterface: read " << output.interface_name << ", Size: " << rpcValue.value().size()
				<< ", Hash: " << std::hash<std::string>{}(rpcValue.value()) << std::endl;
		}
		std::string value = rpcValue.value();
		mapper->mapToInternalState(value, output.interface_name);
	}
	return 0;
}

int OSMPInterface::readFromInternalState() {

	for (auto input : config.inputs) {
		// context to handle the following rpc call
		std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);
		auto namedValue = CoSiMa::rpc::NamedBytes();
		namedValue.set_name(input.interface_name);
		std::string value = mapper->mapFromInternalState(input.interface_name);
		namedValue.set_value(value);

		CoSiMa::rpc::Int32 rpcRetVal;
		if (verbose) {
			std::cout << "OSMPInterface: write " << input.interface_name << ", Size: " << value.size()
				<< ", Hash: " << std::hash<std::string>{}(value) << std::endl;
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

void OSMPInterface::stopSimulation() {
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);
	auto rpcBool = CoSiMa::rpc::Bool();
	CoSiMa::rpc::Bool rpcValue;
	stub->Close(context.get(), rpcBool, &rpcValue);
}

std::string OSMPInterface::getSensorViewConfigurationRequest() {
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);
	auto string = CoSiMa::rpc::String();
	string.set_value("OSMPSensorViewInConfigRequest");

	CoSiMa::rpc::Bytes rpcValue;
	auto status = stub->GetStringValue(context.get(), string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg << std::endl;
		throw std::exception();
	}
	return rpcValue.value();
}

void OSMPInterface::setSensorViewConfiguration(std::string& appliedsensorviewconfiguration) {
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);
	auto namedString = CoSiMa::rpc::NamedBytes();
	namedString.set_name("OSMPSensorViewInConfig");
	namedString.set_value(appliedsensorviewconfiguration);

	CoSiMa::rpc::Int32 rpcValue;
	auto status = stub->SetStringValue(context.get(), namedString, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg << std::endl;
		throw std::exception();
	}
}

int OSMPInterface::disconnect() {
	std::cout << "OSMPInterface::disconnect()" << std::endl;
	return 0;
}
