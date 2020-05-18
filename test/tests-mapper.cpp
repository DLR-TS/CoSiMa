#include "catch2/catch.hpp"

#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "MockBaseSimulator.cpp"
#include "MockInterfaceSimulator.cpp"
#include "MockMapper.cpp"
#include "configreader/StandardYAMLConfig.h"

TEST_CASE("Request variable from base system") {

	MockBaseSimulator* basesimulator= new MockBaseSimulator();
	Mapper* mapper = (Mapper*)new MockMapper();
	std::shared_ptr<BaseSystemInterface> base_simulator_ptr = std::shared_ptr<BaseSystemInterface>((BaseSystemInterface*) basesimulator);

	InterfaceYAMLConfig config;
	config.ip = "12.34.56.78";
	config.port = 1000;
	std::vector<VariableDefinition> inputs;
	std::vector<VariableDefinition> outputs;
	
	VariableDefinition definition;
	definition.base_name = "VERYBASENAME";
	definition.interface_name = "MUCHINTERFACE";
	definition.type = "string";
	inputs.push_back(definition);

	config.inputs = inputs;
	config.outputs = outputs;

	mapper->readConfiguration(config);
	mapper->searchInput(base_simulator_ptr);

	REQUIRE(basesimulator->requestedVariables.size() == 1);
	REQUIRE(basesimulator->requestedVariables.at(0) == "VERYBASENAME");
}

TEST_CASE("Mapping in internalstate the outputs of interfaces") {
	MockBaseSimulator* basesimulator = new MockBaseSimulator();
	MockMapper* n = new MockMapper();
	Mapper* mapper = (Mapper*)n;
	MockInterfaceSimulator* interface_simulator = new MockInterfaceSimulator(std::shared_ptr<Mapper>(mapper));
	std::shared_ptr<BaseSystemInterface> base_simulator_ptr = std::shared_ptr<BaseSystemInterface>((BaseSystemInterface*)basesimulator);

	InterfaceYAMLConfig config;
	config.ip = "12.34.56.78";
	config.port = 1000;
	std::vector<VariableDefinition> inputs;
	std::vector<VariableDefinition> outputs;

	VariableDefinition definition;
	definition.base_name = "VERYBASENAME";
	definition.interface_name = "MUCHINTERFACE";
	definition.type = "string";
	outputs.push_back(definition);

	VariableDefinition definition2;
	definition2.base_name = "VERYBASENAME2";
	definition2.interface_name = "MUCHINTERFACE2";
	definition2.type = "string";
	outputs.push_back(definition2);

	config.inputs = inputs;
	config.outputs = outputs;

	mapper->readConfiguration(config);
	//Problems:
	//mapper->setOwner(std::shared_ptr<iSimulationData>((iSimulationData*)interface_simulator));

	//mapper->mapIn("CRAZYVALUE", "MUCHINTERFACE", eDataType::STRINGCOSIMA);
	//mapper->mapIn("CRAZYVALUE123", "MUCHINTERFACE2", eDataType::STRINGCOSIMA);

	//REQUIRE(interface_simulator->getInternalState()->strings.at(0) == "CRAZYVALUE");
	//REQUIRE(interface_simulator->getInternalState()->strings.at(1) == "CRAZYVALUE123");
}
