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
	grpc::ChannelArguments channelArgs;
	channelArgs.SetMaxSendMessageSize(-1);
	channelArgs.SetMaxReceiveMessageSize(-1);
	channel = grpc::CreateCustomChannel(sstr.str(), grpc::InsecureChannelCredentials(), channelArgs);
	stub = CoSiMa::rpc::BaseInterface::NewStub(channel);
	configStub = CoSiMa::rpc::CARLAInterface::NewStub(channel);

	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.initialisationTransactionTimeout);

	CoSiMa::rpc::CarlaConfig rpcConfig = parseConfigToGRPC();

	CoSiMa::rpc::Int32 response;

	//TODO does this call take ownership of the context, thus freeing it twice? (would need context.release() instead of context.get() to prevent double free)
	auto status = configStub->SetConfig(context.get(), rpcConfig, &response);


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
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
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
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcValue.value();
}

int CARLAInterface::getIntValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Int32 rpcValue;

	auto status = stub->GetIntValue(context.get(), string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcValue.value();
};

bool CARLAInterface::getBoolValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Bool rpcValue;

	auto status = stub->GetBoolValue(context.get(), string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcValue.value();
};

float CARLAInterface::getFloatValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Float rpcValue;

	auto status = stub->GetFloatValue(context.get(), string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcValue.value();
};

double CARLAInterface::getDoubleValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Double rpcValue;

	auto status = stub->GetDoubleValue(context.get(), string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}


	return rpcValue.value();
};

std::string CARLAInterface::getStringValue(std::string base_name) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Bytes rpcValue;

	auto status = stub->GetStringValue(context.get(), string, &rpcValue);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcValue.value();
};

int CARLAInterface::setIntValue(std::string base_name, int value) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto namedValue = CoSiMa::rpc::NamedInt32();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetIntValue(context.get(), namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcRetVal.value();
};

int CARLAInterface::setBoolValue(std::string base_name, bool value) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto namedValue = CoSiMa::rpc::NamedBool();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetBoolValue(context.get(), namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcRetVal.value();
};

int CARLAInterface::setFloatValue(std::string base_name, float value) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto namedValue = CoSiMa::rpc::NamedFloat();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetFloatValue(context.get(), namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcRetVal.value();
};

int CARLAInterface::setDoubleValue(std::string base_name, double value) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto namedValue = CoSiMa::rpc::NamedDouble();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetDoubleValue(context.get(), namedValue, &rpcRetVal);

	if (!status.ok()) {
		auto msg = status.error_message();
		std::cerr << msg;
#ifdef __linux__
		throw std::exception();
#else
		throw std::exception(msg.c_str());
#endif
	}

	return rpcRetVal.value();
};

int CARLAInterface::setStringValue(std::string base_name, std::string value) {
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto namedValue = CoSiMa::rpc::NamedBytes();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

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

	return rpcRetVal.value();
}

CoSiMa::rpc::CarlaConfig CARLAInterface::parseConfigToGRPC()
{
	CoSiMa::rpc::CarlaConfig rpcConfig;
	rpcConfig.set_carla_host(config.carla_host);
	rpcConfig.set_carla_port(config.carla_port);
	rpcConfig.set_transaction_timeout(config.transactionTimeout);
	rpcConfig.set_delta_seconds(config.deltaSeconds);

	for (auto& sensorViewExtra : config.osiSensorViewConfig) {
		auto rpcSensorViewExtra = rpcConfig.add_sensor_view_extras();
		rpcSensorViewExtra->set_prefixed_fmu_variable_name(sensorViewExtra.prefixedFmuVariableName);

		copyMountingPositions(sensorViewExtra.cameraSensorMountingPosition,
			rpcSensorViewExtra->mutable_sensor_mounting_position()->add_camera_sensor_mounting_position());

		copyMountingPositions(sensorViewExtra.radarSensorMountingPosition,
			rpcSensorViewExtra->mutable_sensor_mounting_position()->add_radar_sensor_mounting_position());

		copyMountingPositions(sensorViewExtra.lidarSensorMountingPosition,
			rpcSensorViewExtra->mutable_sensor_mounting_position()->add_lidar_sensor_mounting_position());

		copyMountingPositions(sensorViewExtra.ultrasonicSensorMountingPosition,
			rpcSensorViewExtra->mutable_sensor_mounting_position()->add_ultrasonic_sensor_mounting_position());

		copyMountingPositions(sensorViewExtra.genericSensorMountingPosition,
			rpcSensorViewExtra->mutable_sensor_mounting_position()->add_generic_sensor_mounting_position());
	}

	return rpcConfig;
}

void CARLAInterface::copyMountingPositions(const std::vector<OSIMountingPosition>& mountingPositions, osi3::MountingPosition * rpcMountingPosition)
{
	for (auto mountingPosition : mountingPositions) {
		auto position = rpcMountingPosition->mutable_position();
		position->set_x(mountingPosition.x);
		position->set_y(mountingPosition.y);
		position->set_z(mountingPosition.z);
		auto orientation = rpcMountingPosition->mutable_orientation();
		orientation->set_pitch(mountingPosition.pitch);
		orientation->set_yaw(mountingPosition.yaw);
		orientation->set_roll(mountingPosition.roll);
	}
}

