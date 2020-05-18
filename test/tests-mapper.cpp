#include "catch2/catch.hpp"

#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"

TEST_CASE("Read simulator names from config") {
	std::shared_ptr<Mapper> mapper = std::shared_ptr<Mapper>((Mapper*)new FMIMapper());

	//REQUIRE(names.size() == 3);
}

TEST_CASE("set configuration of simulator") {

}

//readConfiguration + mapIn möglich
//erstellen von Mocks für Basesystem und Interfacesystem
