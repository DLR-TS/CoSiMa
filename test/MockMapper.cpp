#include "catch2/catch.hpp"

#include "mapper/Mapper.h"

class MockMapper : public Mapper {

	void mapTo(values_t value, std::string interfaceName, eDataType type) {

	}

};
