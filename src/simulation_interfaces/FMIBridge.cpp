#include <string>
#include "simulation_interfaces/FMIBridge.h"

int FMIBridge::init(std::string scenario, float starttime, int mode) {

	coSimFMUInstance = coSimFMU->new_instance();
	//TODO where to get the stop time and tolerance?
	//coSimFMUInstance->setup_experiment((fmi2Real) starttime,)
	//TODO set variables with initial==exact or initial==approx
	coSimFMUInstance->enter_initialization_mode();
	//TODO set independent and continuous-time variables with initial==exact
	//TODO set continuous- and discrete-time inputs and optionally also the derivatives of the former
	//TODO get (calculated or dependent) values and derivatives, if needed
	coSimFMUInstance->exit_initialization_mode();
	return 0;
}

int FMIBridge::connect(std::string) {
	return 0;
}

int FMIBridge::disconnect() {
	return 0;
}

int FMIBridge::readOutputs() {
	//for (auto const& outputVar : *(coSimFMU->get_model_description()->model_variables)) {
	//	//TODO cache output variables instead of filtering every time?
	//	if (fmi4cpp::fmi2::causality::output == outputVar.causality) {

	auto const model_description = coSimFMU->get_model_description();
	//iterate over unknowns declared as output
	for(auto const& unknown: model_description->model_structure->outputs) {
		// translate unknown into scalar_variable
		auto const& outputVar = (*model_description->model_variables.get())[unknown.index];

		// cannot switch on outputVar.value_type because fmi4cpp uses strings
		// Map output value into internal state
		if (outputVar.is_boolean()) {
			fmi2Boolean boolean;
			coSimFMUInstance->read_boolean(outputVar.value_reference, boolean);
			mapper->mapIn(boolean, outputVar.name, eDataType::BOOLCOSIMA);
		}
		else if (outputVar.is_enumeration() || outputVar.is_integer()) {
			fmi2Integer integer;
			coSimFMUInstance->read_integer(outputVar.value_reference, integer);
			mapper->mapIn(integer, outputVar.name, eDataType::INTEGERCOSIMA);
		}
		else if (outputVar.is_real()) {
			fmi2Real real;
			coSimFMUInstance->read_real(outputVar.value_reference, real);
			mapper->mapIn(real, outputVar.name, eDataType::DOUBLECOSIMA);
		}
		else {
			fmi2String string;
			coSimFMUInstance->read_string(outputVar.value_reference, string);
			mapper->mapIn(string, outputVar.name, eDataType::STRINGCOSIMA);
		}
		//}
	}
	return 0;
}

int FMIBridge::doStep(double stepSize) {
	//TODO set independent tunable parameters
	//TODO set contidouble stepSize = 1nuous- and discrete-time inputs and optionally also the derivatives of the former

	//TODO step by stepSize
	coSimFMUInstance->step(stepSize);

	//TODO get variables -- might not be needed here and instead be served by readOutputs

	return 0;
}
