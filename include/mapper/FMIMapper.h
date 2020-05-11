#ifndef FMIMAPPER_H
#define FMIMAPPER_H

#include "Mapper.h"

class FMIMapper : Mapper {

public:
	FMIMapper() : Mapper() {};
	int readConfiguration(configVariants_t config) override;

	void setConfiguration(/*FMICOnfig*/);
	void mapTo(values_t value, std::string interfaceName, DataTypes type) override;

};

#endif // !FMIMAPPER_H
