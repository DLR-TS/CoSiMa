#include "simulation_interfaces/SUMOInterface.h"
using namespace LIBSUMO_NAMESPACE;

void SUMOInterface::configure(const YAML::Node& node) {
	config = node.as<SUMOInterfaceConfig>();
	configureMapperOwner();
	mapper->readConfiguration(config);
}

int SUMOInterface::init(bool verbose, std::string configurationPath) {
	if (verbose) {
		std::cout << "SUMO Version: " << Simulation::getVersion().second << std::endl;
	}
	std::vector<std::string> cmd;
	cmd.push_back("sumo");
	cmd.push_back("-c");
	cmd.push_back(configurationPath + config.model);
	Simulation::start(cmd);
	return 0;
}

int SUMOInterface::disconnect() {
	Simulation::close();
	return 0;
}

int SUMOInterface::writeToInternalState() {
	osi3::TrafficUpdate trafficUpdate;
	//get data from SUMO
	const std::vector<std::string> vehicleIds = Simulation::getLoadedIDList();
	//ID from Vehicle or Pedestrian?

	for (auto& id: vehicleIds) {
		//https://sumo.dlr.de/docs/TraCI/Vehicle_Value_Retrieval.html

		//https://sumo.dlr.de/docs/TraCI/Person_Value_Retrieval.html

		auto* movingObject = trafficUpdate.add_update();
		movingObject->mutable_id()->set_value(std::stoul(id));
		osi3::BaseMoving* base = movingObject->mutable_base();
				
		base->mutable_dimension()->set_height(Vehicle::getHeight(id));
		base->mutable_dimension()->set_width(Vehicle::getWidth(id));
		base->mutable_dimension()->set_length(Vehicle::getLength(id));

		auto pos =  Vehicle::getPosition(id);
		base->mutable_position()->set_x(pos.x);
		base->mutable_position()->set_y(pos.y);

		double angleRadians = Vehicle::getAngle(id) * (M_PI / 180.0);
		base->mutable_orientation()->set_yaw(angleRadians);

		auto speed = Vehicle::getSpeed(id);
		base->mutable_velocity()->set_x(speed * cos(angleRadians));
		base->mutable_velocity()->set_y(speed * sin(angleRadians));

		double acceleration = Vehicle::getAcceleration(id);
		base->mutable_acceleration()->set_x(acceleration * cos(angleRadians));
		base->mutable_acceleration()->set_y(acceleration * sin(angleRadians));

		//Vehicle::getVehicleClass(id);
		std::cout << Vehicle::getVehicleClass(id) << std::endl;
	}
	std::string tuString = trafficUpdate.SerializeAsString();
	for (auto& output : config.outputs) {
		mapper->mapToInternalState(tuString, output.interface_name);
	}
	return 0;
}

int SUMOInterface::readFromInternalState() {
	//write data to SUMO
	return 0;
}

int SUMOInterface::doStep(double stepSize) {
	Simulation::step(stepSize);
	return 0;
}

void SUMOInterface::stopSimulation() {
	Simulation::close("Close by OSTAR CoSimulationManager");
}
