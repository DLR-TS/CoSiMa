#ifndef ROSMAPPER_H
#define ROSMAPPER_H

#include "Mapper.h"

class ROSMapper : Mapper {

	void mapTo(values_t value, std::string interfaceName, eDataType type) override;
};

#endif // !ROSMAPPER_H
