#ifndef VTDMAPPER_H
#define VTDMAPPER_H

#include "Mapper.h"

class VTDMapper : Mapper {

	void mapTo(values_t value, std::string interfaceName, DataTypes type) override;
};

#endif // !VTDMAPPER_H
