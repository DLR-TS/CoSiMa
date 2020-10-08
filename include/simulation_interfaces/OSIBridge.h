#ifndef OSIBRIDGE_H
#define OSIBRIDGE_H

#include <map>
#include <string>
#include "iSimulationData.h"
#include "../Mapper/Mapper.h"
#include "../mapper/OSIMapper.h"
#include "OSIMessages.h"
#include "osi_sensorview.pb.h"
#include "osi_trafficcommand.pb.h"
#include "osi_trafficupdate.pb.h"
#include "sl45_motioncommand.pb.h"
#include "sl45_vehiclecommunicationdata.pb.h"


class OSIBridge : public iSimulationData
{
public:
	OSIBridge(std::shared_ptr<Mapper> mapper) : iSimulationData(mapper) {};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int writeToInternalState() override;
	/**
	Write osi message from FMU as string representation to internalstate
	\param address address Information about the osi message location, type and name
	\return success indicator
	*/
	int writeToInternalState(address address);
	int readFromInternalState() override;
	/**
	Write osi message from internalstate to the given message location for the FMU
	\param address address Information about the osi message location, type and name
	\return success indicator
	*/
	int readFromInternalState(address& address);
	int doStep(double stepSize = 1) override;

	int readConfiguration(configVariants_t configVariants) override;

	/**
	Parse string value to OSI Message Enum.
	\param std::string messageType string to parse into its enum value representation
	\return messagetype enum value of given string
	*/
	eOSIMessage getMessageType(std::string messageType);

protected:
	osi3::SensorView sensorView;
	osi3::SensorViewConfiguration sensorViewConfiguration;
	osi3::SensorData sensorData;
	osi3::GroundTruth groundTruth;
	osi3::TrafficCommand trafficCommand;
	osi3::TrafficUpdate trafficUpdate;
	setlevel4to5::MotionCommand motionCommand;
	setlevel4to5::VehicleCommunicationData vehicleCommunicationData;

	/**
	* OSI message type and its location
	*/
	std::map<eOSIMessage, address> writeAddressInformation;
	std::map<eOSIMessage, address> readAddressInformation;
};

#endif // !OSIBRIDGE_H
