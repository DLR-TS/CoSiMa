#ifndef OSIMAPPER_H
#define OSIMAPPER_H

#include "Mapper.h"
#include "simulation_interfaces/OSIMessages.h"
#include "sl45_motioncommand.pb.h"
#include "sl45_vehiclecommunicationdata.pb.h"

class OSIMapper : public Mapper {

public:
	OSIMapper() : Mapper() {};
	int readConfiguration(configVariants_t config) override;

	/** Prefix string for cosima to evaluate content as an OSI Message
	*/
	std::string prefix = "#";
}; 

#endif // !OSIMAPPER_H
