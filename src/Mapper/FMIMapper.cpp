#include "mapper/FMIMapper.h"

void FMIMapper::mapTo(values_t value, std::string interfaceName, eDataType type) {

}

void FMIMapper::parseFMU2ModelDescription(fmi4cpp::fmi2::model_description& descr)
{
	descr.variable_naming_convention
}

void FMIMapper::setConfiguration() {

}

int FMIMapper::readConfiguration(configVariants_t configVariants) {
	std::cout << "Read Configuration of FMI Mapper" << std::endl;
	return 0;
}