#ifndef FMIMAPPER_H
#define FMIMAPPER_H

#include "Mapper.h"

class FMIMapper : Mapper {

	void setConfiguration(/*FMICOnfig*/);
	void mapTo(values_t value, std::string interfaceName, DataTypes type) override;
};

#endif // !FMIMAPPER_H
