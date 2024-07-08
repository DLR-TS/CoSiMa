/**
@authors German Aerospace Center: Björn Bahn
*/

#ifndef SUMOMAPPER_H
#define SUMOMAPPER_H

#include "Mapper.h"
#include "simulation_interfaces/OSIMessages.h"

class SUMOMapper : public Mapper {

public:
	SUMOMapper() : Mapper() {};
	int readConfiguration(configVariants_t config) override;
};

#endif // !SUMOMAPPER_H