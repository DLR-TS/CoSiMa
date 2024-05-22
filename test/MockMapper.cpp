/**
@authors German Aerospace Center: Bj�rn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "mapper/Mapper.h"

class MockMapper : public Mapper {

	void mapToInternalState(std::string& value, std::string& interfaceName) {};

};
