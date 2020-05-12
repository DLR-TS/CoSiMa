#ifndef SUMOMAPPER_H
#define SUMOMAPPER_H

#include "Mapper.h"

class SUMOMapper : Mapper {

	void mapTo(values_t value, std::string interfaceName, eDataType type) override;
};

#endif // !SUMOMAPPER_H
