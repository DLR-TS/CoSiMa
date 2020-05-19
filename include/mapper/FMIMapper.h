#ifndef FMIMAPPER_H
#define FMIMAPPER_H

#include "Mapper.h"
#include "fmi4cpp/fmi4cpp.hpp"

class FMIMapper : Mapper {

public:
	FMIMapper() : Mapper() {};
	int readConfiguration(configVariants_t config) override;

	void setConfiguration(/*FMIConfig*/);
	void mapTo(values_t value, std::string interfaceName, eDataType type) override;

private:
	void parseFMU2ModelDescription(fmi4cpp::fmi2::model_description& descr);

};

#endif // !FMIMAPPER_H
