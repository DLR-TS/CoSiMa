#ifndef FMIMAPPER_H
#define FMIMAPPER_H

#include "Mapper.h"

class FMIMapper : Mapper {

public:
	FMIMapper() : Mapper() {};
	int readConfiguration(configVariants_t config) override;

	void setConfiguration(/*FMICOnfig*/);

};

#endif // !FMIMAPPER_H
