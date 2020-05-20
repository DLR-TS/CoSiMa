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

	//test begin
	mapper->readConfiguration(config);
	mapper->searchInput(base_simulator_ptr);

	REQUIRE(basesimulator->requestedVariables.size() == 1);
	REQUIRE(basesimulator->requestedVariables.at(0) == "VERYBASENAME");
}

TEST_CASE("Mapping the outputs of interfaces to the internalstate") {
	MockBaseSimulator* basesimulator = new MockBaseSimulator();
	MockMapper* n = new MockMapper();
	Mapper* mapper = (Mapper*)n;
	std::shared_ptr<iSimulationData> interface_simulator = std::shared_ptr<iSimulationData>(new MockInterfaceSimulator(std::shared_ptr<Mapper>(mapper)));
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
	definition2.type = "int";
	outputs.push_back(definition2);

	config.inputs = inputs;
	config.outputs = outputs;

	interface_simulator->getMapper()->setOwner(interface_simulator);
	interface_simulator->getMapper()->readConfiguration(config);
	
	std::string value1 = "CRAZYVALUE";
	int value2 = 1289;

	//test begin
	interface_simulator->getMapper()->mapIn(value1, "MUCHINTERFACE", eDataType::STRINGCOSIMA);
	interface_simulator->getMapper()->mapIn(value2, "MUCHINTERFACE2", eDataType::INTEGERCOSIMA);

	REQUIRE(interface_simulator->getInternalState()->strings.at(0) == "CRAZYVALUE");
	REQUIRE(interface_simulator->getInternalState()->integers.at(0) == 1289);
}

TEST_CASE(){
	MockBaseSimulator* basesimulator = new MockBaseSimulator();
	MockMapper* n = new MockMapper();
	Mapper* mapper = (Mapper*)n;
	std::shared_ptr<iSimulationData> interface_simulator = std::shared_ptr<iSimulationData>(new MockInterfaceSimulator(std::shared_ptr<Mapper>(mapper)));
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

	config.inputs = inputs;
	config.outputs = outputs;

	interface_simulator->getMapper()->setOwner(interface_simulator);
	interface_simulator->getMapper()->readConfiguration(config);

	std::string value = "testvalue";
	mapper->mapIn(value, "MUCHINTERFACE", STRINGCOSIMA);

	//test begin
	mapper->writeOutput(base_simulator_ptr);

	REQUIRE(basesimulator->stringvalue == "testvalue");
};
