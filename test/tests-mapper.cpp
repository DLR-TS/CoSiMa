/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#include "catch2/catch.hpp"

#include "mapper/Mapper.h"
#include "MockBaseSimulator.cpp"
#include "MockInterfaceSimulator.cpp"
#include "MockMapper.cpp"
#include "reader/StandardYAMLConfig.h"

TEST_CASE("Request variable from base system") {

	std::shared_ptr<MockBaseSimulator> basesimulator = std::make_shared<MockBaseSimulator>();
	std::shared_ptr<Mapper> mapper = std::make_shared<Mapper>();
	std::shared_ptr<BaseSystemInterface> base_simulator_ptr = basesimulator;
	std::shared_ptr<SimulatorInterface> interface_simulator = std::make_shared<MockInterfaceSimulator>(mapper);

	InterfaceYAMLConfig config;
	config.ip = "12.34.56.78";
	config.port = 1000;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;

	ConfigParameter definition;
	definition.base_name = "VERYBASENAME";
	definition.interface_name = "MUCHINTERFACE";
	inputs.push_back(definition);

	config.inputs = inputs;
	config.outputs = outputs;

	mapper->setOwner(interface_simulator);

	//test begin
	mapper->readConfiguration(config);
	mapper->searchInput(base_simulator_ptr);

	REQUIRE(basesimulator->requestedVariables.size() == 1);
	REQUIRE(basesimulator->requestedVariables.at(0) == "VERYBASENAME");
}

TEST_CASE("Mapping the outputs of interfaces to the internalstate") {
	/*std::shared_ptr<MockBaseSimulator> basesimulator = std::make_shared<MockBaseSimulator>();
	std::shared_ptr<Mapper> mapper = std::make_shared<MockMapper>();
	std::shared_ptr<SimulatorInterface> interface_simulator = std::make_shared<MockInterfaceSimulator>(mapper);

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
	interface_simulator->getMapper()->mapToInternalState(value1, "MUCHINTERFACE", eDataType::STRINGCOSIMA);
	interface_simulator->getMapper()->mapToInternalState(value2, "MUCHINTERFACE2", eDataType::INTEGERCOSIMA);

	REQUIRE(interface_simulator->getMapper()->getMessageCache()->strings.at(0) == "CRAZYVALUE");
	REQUIRE(interface_simulator->getMapper()->getMessageCache()->integers.at(0) == 1289);*/
}

TEST_CASE() {
	std::shared_ptr<MockBaseSimulator> basesimulator = std::make_shared<MockBaseSimulator>();
	std::shared_ptr<Mapper> mapper = std::make_shared<MockMapper>();
	std::shared_ptr<SimulatorInterface> interface_simulator = std::make_shared<MockInterfaceSimulator>(mapper);
	std::shared_ptr<BaseSystemInterface> base_simulator_ptr = basesimulator;

	InterfaceYAMLConfig config;
	config.ip = "12.34.56.78";
	config.port = 1000;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;

	ConfigParameter definition;
	definition.base_name = "VERYBASENAME";
	definition.interface_name = "MUCHINTERFACE";
	outputs.push_back(definition);

	config.inputs = inputs;
	config.outputs = outputs;

	/*interface_simulator->configure(); getMapper()->setOwner(interface_simulator);
	interface_simulator->getMapper()->readConfiguration(config);

	std::string value = "testvalue";
	mapper->mapToInternalState(value, "MUCHINTERFACE", STRINGCOSIMA);

	//test begin
	mapper->writeOutput(base_simulator_ptr);

	REQUIRE(basesimulator->stringvalue == "testvalue");*/
};

TEST_CASE("Read a YAML-config into internal state and back") {
	std::shared_ptr<MockBaseSimulator> basesimulator = std::make_shared<MockBaseSimulator>();
	std::shared_ptr<Mapper> mapper = std::make_shared<MockMapper>();
	std::shared_ptr<SimulatorInterface> interface_simulator = std::make_shared<MockInterfaceSimulator>(mapper);
	std::shared_ptr<BaseSystemInterface> base_simulator_ptr = basesimulator;

	InterfaceYAMLConfig config;
	config.ip = "12.34.56.78";
	config.port = 1000;
	std::vector<ConfigParameter> inputs;
	std::vector<ConfigParameter> outputs;

	//test values
	std::string stringValue = "A string for CoSiMa";
	bool boolValue = true;
	int integerValue = -1234;
	float floatValue = -1.234f;
	double doubleValue = 4.567;

	ConfigParameter definition;
	std::vector<std::string> inputVarNames;
	std::vector<std::string> outputVarNames;

	basesimulator->stringvalue = stringValue;
	definition.base_name = "base.string_in";
	definition.interface_name = "string_in";
	inputs.push_back(definition);
	inputVarNames.push_back(definition.interface_name);
	definition.base_name = "base.string_out";
	definition.interface_name = "string_out";
	outputs.push_back(definition);
	outputVarNames.push_back(definition.interface_name);

	config.inputs = inputs;
	config.outputs = outputs;
}
