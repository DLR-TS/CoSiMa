#ifndef UNREALMAPPER_H
#define UNREALMAPPER_H

#include "Mapper.h"

class UnrealMapper : Mapper {

	void mapTo(values_t value, std::string interfaceName, DataTypes type) override;
};

#endif // !UNREALMAPPER_H
