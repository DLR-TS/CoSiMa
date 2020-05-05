#ifndef ROSMAPPER_H
#define ROSMAPPER_H

#include "Mapper.h"

class ROSMapper : Mapper {

	void mapTo(values_t value, std::string interfaceName, DataTypes type) override;
};

#endif // !ROSMAPPER_H
