#ifndef OSMPBRIDGE_H
#define OSMPBRIDGE_H
#define NOMINMAX
#include "fmi4cpp/fmi4cpp.hpp"
#include "iSimulationData.h"
#include "../Mapper/Mapper.h"
#include "../mapper/OSIMapper.h"
#include "OSIMessages.h"
#include "OSIBridge.h"
#include "FMIBridge.h"

class OSMPBridge : public OSIBridge
{
public:
	OSMPBridge(std::shared_ptr<Mapper> mapper) : OSIBridge(mapper){};

	int init(std::string scenario, float starttime, int mode) override;
	int connect(std::string) override;
	int disconnect() override;

	int writeToInternalState() override;
	int readFromInternalState() override;
	int doStep(double stepSize = 1) override;

	/**
	Save the annotated value in the address map. Supported names are count, valid, <>.base.hi , <>.base.lo, <>.size.
	\param std::map<std::string, address> &addressMap The map, the value is mapped in.
	\param std::string name name of the variable. Supported names are count, valid, <>.base.hi , <>.base.lo, <>.size.
	\param int value The value to be stored.
	*/
	void saveToAddressMap(std::map<std::string, address> &addressMap, std::string name, int value);

	/**
	Parse string value to OSI Message Enum.
	\param std::string messageType string to parse into its enum value representation
	\return messagetype enum value of given string
	*/
	eOSIMessage getMessageType(std::string messageType);

	/**
	Temporary storage for osmp messages (name, size, address)
	*/
	std::map<std::string, address> inputAddresses, outputAddresses;

	/**
	Read configuration and fill simulation configuration.
	\param config the decoding struct
	\return success status
	*/
	int readConfiguration(configVariants_t configVariants) override;

	/**
	extracts the index of the prefix
	\param std::string string with potential \[<index>\] at the end
	\return Index of data entry (positive value). No index: -1
	*/
	int extractIndex(std::string name);
protected:
	class OSMPFMUSlaveStateWrapper {
	private:
		//cs_slave creating this fmu state is needed later for freeing the memory again
		std::shared_ptr<fmi4cpp::fmi2::cs_slave> coSimSlave;

		OSMPFMUSlaveStateWrapper(std::shared_ptr < fmi4cpp::fmi2::cs_slave> slave);

	public:
		~OSMPFMUSlaveStateWrapper();

		fmi4cpp::fmi4cppFMUstate state;
		static std::optional<OSMPFMUSlaveStateWrapper> tryGetStateOf(std::shared_ptr<fmi4cpp::fmi2::cs_slave> slave);
	};

private:
	//fmi4cpp::fmi4cppFMUstate state;
	std::unique_ptr<fmi4cpp::fmi2::cs_fmu> coSimFMU;
	std::shared_ptr<fmi4cpp::fmi2::cs_slave> coSimSlave;

	/**
	stores the field \"valid\" from fmi
	*/
	bool valid = true;
	/**
	stores the field \"count\" from fmi
	*/
	int count;
};

#endif // !OSMPBRIDGE_H
