#include "base_interfaces/CARLAInterface.h"

int CARLAInterface::readConfiguration(baseConfigVariants_t variant) {
	if (std::get_if<CARLAInterfaceConfig>(&variant) == nullptr) {
		std::cerr << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}
	config = std::get<CARLAInterfaceConfig>(variant);

	return 0;
}

int CARLAInterface::initialize(bool verbose) {
	this->verbose = verbose;
	std::ostringstream sstr;
	sstr << config.client_host << ':' << config.client_port;
	grpc::ChannelArguments channelArgs;
	channelArgs.SetMaxSendMessageSize(-1);
	channelArgs.SetMaxReceiveMessageSize(-1);
	channel = grpc::CreateCustomChannel(sstr.str(), grpc::InsecureChannelCredentials(), channelArgs);
	stub = CoSiMa::rpc::BaseInterface::NewStub(channel);
	configStub = CoSiMa::rpc::CARLAInterface::NewStub(channel);

	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.initializationTransactionTimeout);

	CoSiMa::rpc::CarlaConfig rpcConfig = parseConfigToGRPC();

	CoSiMa::rpc::Int32 response;

	auto status = configStub->SetConfig(context.get(), rpcConfig, &response);
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

double CARLAInterface::doStep(double stepSize)
{
	// context to handle the following rpc call - cannot be reused
	std::unique_ptr<grpc::ClientContext> context = CoSiMa::Utility::CreateDeadlinedClientContext(config.doStepTransactionTimeout);

	auto empty = CoSiMa::rpc::Empty();
	CoSiMa::rpc::Double rpcValue;
	//Empty is intepreted as 1
	auto status = stub->DoStep(context.get(), empty, &rpcValue);

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
	auto entry = integerMap.find(base_name);
	if (integerMap.end() == entry) {
		if (verbose) {
			std::cout << "CarlaInterface: getIntValue(" << base_name << ") No variable found.\n";
		}
		return 0;
	}
	return entry->second;
};

bool CARLAInterface::getBoolValue(std::string base_name) {
	auto entry = boolMap.find(base_name);
	if (boolMap.end() == entry) {
		if (verbose) {
			std::cout << "CarlaInterface: getBoolValue(" << base_name << ") No variable found.\n";
		}
		return 0;
	}
	return entry->second;
};

float CARLAInterface::getFloatValue(std::string base_name) {
	auto entry = floatMap.find(base_name);
	if (floatMap.end() == entry) {
		if (verbose) {
			std::cout << "CarlaInterface: getFloatValue(" << base_name << ") No variable found.\n";
		}
		return 0;
	}
	return entry->second;
};

double CARLAInterface::getDoubleValue(std::string base_name) {
	auto entry = doubleMap.find(base_name);
	if (doubleMap.end() == entry) {
		if (verbose) {
			std::cout << "CarlaInterface: getDoubleValue(" << base_name << ") No variable found.\n";
		}
		return 0;
	}
	return entry->second;
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
	integerMap[base_name] = value;
	if (verbose) {
		std::cout << "CarlaInterface: setIntValue() to " << value << "\n";
	}
	return 0;
};

int CARLAInterface::setBoolValue(std::string base_name, bool value) {
	boolMap[base_name] = value;
	if (verbose) {
		std::cout << "CarlaInterface: setBoolValue() to " << value << "\n";
	}
	return 0;
};

int CARLAInterface::setFloatValue(std::string base_name, float value) {
	floatMap[base_name] = value;
	if (verbose) {
		std::cout << "CarlaInterface: setFloatValue() to " << value << "\n";
	}
	return 0;
};

int CARLAInterface::setDoubleValue(std::string base_name, double value) {
	doubleMap[base_name] = value;
	if (verbose) {
		std::cout << "CarlaInterface: setDoubleValue() to " << value << "\n";
	}
	return 0;
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

int CARLAInterface::disconnect() { return 0; };
