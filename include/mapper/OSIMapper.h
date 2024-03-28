/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef OSIMAPPER_H
#define OSIMAPPER_H

#include "Mapper.h"
#include "simulation_interfaces/OSIMessages.h"

class OSIMapper : public Mapper {

public:
	OSIMapper() : Mapper() {};
	int readConfiguration(configVariants_t config) override;
}; 

#endif // !OSIMAPPER_H
