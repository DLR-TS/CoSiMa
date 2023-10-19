#include "base_interfaces/CARLAInterface.h"

void CARLAInterface::configure(YAML::Node node) {
	config = node.as<CARLAInterfaceConfig>();
}

int CARLAInterface::init(bool verbose) {
	this->verbose = verbose;
	std::ostringstream sstr;
	sstr << config.client_host << ':' << config.client_port;
	grpc::ChannelArguments channelArgs;
	channelArgs.SetMaxSendMessageSize(-1);
	channelArgs.SetMaxReceiveMessageSize(-1);
	channel = grpc::CreateCustomChannel(sstr.str(), grpc::InsecureChannelCredentials(), channelArgs);
	stub = CoSiMa::rpc::BaseInterface::NewStub(channel);
	configStub = CoSiMa::rpc::CARLAInterface::NewStub(channel);

	CoSiMa::rpc::CarlaConfig rpcConfig = parseConfigToGRPC();

	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.initializationTransactionTimeout);

	CoSiMa::rpc::Int32 response;

	auto status = configStub->SetConfig(context.get(), rpcConfig, &response);

	if (verbose) {
		std::cout << "Response: " << response.value() << " Status: " << status.ok() << std::endl;
	}
	//Does the context need to be released manually?
	//context.release();

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
		std::cerr << status.error_message() << std::endl;
		throw std::exception();
	}

	return response.value();
}

double CARLAInterface::doStep(double stepSize)
{
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.doStepTransactionTimeout);

	auto empty = CoSiMa::rpc::Empty();
	CoSiMa::rpc::Double rpcValue;
	//Empty is intepreted as 1
	auto status = stub->DoStep(context.get(), empty, &rpcValue);

	if (!status.ok()) {
		std::cerr << status.error_message() << std::endl;
		throw std::exception();
	}

	return rpcValue.value();
}

std::string CARLAInterface::getOSIMessage(const std::string& base_name) {
	if (verbose) {
		std::cout << "Get " << base_name << " from CARLA interface" << std::endl;
	}
	if (base_name == "OSMPSensorData") {
		return save;
	}
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto string = CoSiMa::rpc::String();
	string.set_value(base_name);

	CoSiMa::rpc::Bytes rpcValue;

	auto status = stub->GetStringValue(context.get(), string, &rpcValue);

	if (!status.ok()) {
		std::cerr << status.error_message() << std::endl;
		throw std::exception();
	}

	return rpcValue.value();
}

int CARLAInterface::setOSIMessage(const std::string& base_name, const std::string& value) {
	if (base_name == "OSMPSensorData") {
		save = value;
	}
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.transactionTimeout);

	auto namedValue = CoSiMa::rpc::NamedBytes();
	namedValue.set_name(base_name);
	namedValue.set_value(value);

	CoSiMa::rpc::Int32 rpcRetVal;

	auto status = stub->SetStringValue(context.get(), namedValue, &rpcRetVal);

	if (!status.ok()) {
		std::cerr << status.error_message() << std::endl;
		throw std::exception();
	}

	return rpcRetVal.value();
}

CoSiMa::rpc::CarlaConfig CARLAInterface::parseConfigToGRPC()
{
	CoSiMa::rpc::CarlaConfig rpcConfig;
	rpcConfig.add_runtimeparameter("--carlahost");
	rpcConfig.add_runtimeparameter(config.carla_host);
	rpcConfig.add_runtimeparameter("--carlaport");
	rpcConfig.add_runtimeparameter(std::to_string(config.carla_port));
	rpcConfig.add_runtimeparameter("--transactiontimeout");
	rpcConfig.add_runtimeparameter(std::to_string(config.transactionTimeout));
	rpcConfig.add_runtimeparameter("--deltaseconds");
	rpcConfig.add_runtimeparameter(std::to_string(config.deltaSeconds));

	std::stringstream ss(config.additionalParameters);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> tokens(begin, end);

	for (auto &parameter : tokens) {
		rpcConfig.add_runtimeparameter(parameter);
	}

	for (auto& sensorViewExtra : config.osiSensorViewConfig) {
		auto rpcSensorViewExtra = rpcConfig.add_sensor_view_extras();
		rpcSensorViewExtra->set_prefixed_fmu_variable_name(sensorViewExtra.prefixedFmuVariableName);

		if (sensorViewExtra.cameraSensorMountingPosition.size()) {
			if (verbose) {
				std::cout << "Set CameraSensorMountingPositon\n";
			}
			copyMountingPositions(sensorViewExtra.cameraSensorMountingPosition,
				rpcSensorViewExtra->mutable_sensor_mounting_position()->add_camera_sensor_mounting_position());
		}
		if (sensorViewExtra.radarSensorMountingPosition.size()) {
			if (verbose) {
				std::cout << "Set RadarSensorMountingPosition\n";
			}
			copyMountingPositions(sensorViewExtra.radarSensorMountingPosition,
				rpcSensorViewExtra->mutable_sensor_mounting_position()->add_radar_sensor_mounting_position());
		}
		if (sensorViewExtra.lidarSensorMountingPosition.size()) {
			if (verbose) {
				std::cout << "Set LidarSensorMountingPosition\n";
			}
			copyMountingPositions(sensorViewExtra.lidarSensorMountingPosition,
				rpcSensorViewExtra->mutable_sensor_mounting_position()->add_lidar_sensor_mounting_position());
		}
		if (sensorViewExtra.ultrasonicSensorMountingPosition.size()) {
			if (verbose) {
				std::cout << "Set UltrasonicSensorMountingPosition\n";
			}
			copyMountingPositions(sensorViewExtra.ultrasonicSensorMountingPosition,
				rpcSensorViewExtra->mutable_sensor_mounting_position()->add_ultrasonic_sensor_mounting_position());
		}
		if (sensorViewExtra.genericSensorMountingPosition.size()) {
			if (verbose) {
				std::cout << "Set GenericSensorMountingPosition\n";
			}
			copyMountingPositions(sensorViewExtra.genericSensorMountingPosition,
				rpcSensorViewExtra->mutable_sensor_mounting_position()->add_generic_sensor_mounting_position());
		}
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

double CARLAInterface::getStepSize() {
	return config.deltaSeconds;
}

int CARLAInterface::disconnect() {
	return 0;
}
