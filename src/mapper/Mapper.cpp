#include "mapper/Mapper.h"

AnnotatedMessage convertToAnnotatedMessage(ConfigParameter& definition) {
	AnnotatedMessage annotatedMessage;
	annotatedMessage.base_name = definition.base_name;
	annotatedMessage.interface_name = definition.interface_name;
	annotatedMessage.message = "";
	return annotatedMessage;
};

void Mapper::searchInput(std::shared_ptr<BaseSystemInterface> baseInterface) {
	for (AnnotatedMessage &input : data.messageInputList) {
		input.message = baseInterface->getOSIMessage(input.base_name);
	}
}

void Mapper::writeOutput(std::shared_ptr<BaseSystemInterface> baseInterface) {
	for (AnnotatedMessage &output : data.messageOutputList) {
		baseInterface->setOSIMessage(output.base_name, output.message);
	}
}

int Mapper::readConfiguration(configVariants_t configVariants) {

#if defined(_WIN32) && (_MSC_VER >= 1910) || defined(__linux__) && __cplusplus >= 201703L
	InterfaceYAMLConfig yamlconfig = std::get<InterfaceYAMLConfig>(configVariants);
#elif defined(_WIN32) && (_MSC_VER >= 1600) || defined(__linux__) && __cplusplus >= 201103L
	InterfaceYAMLConfig yamlconfig = boost::get<InterfaceYAMLConfig>(configVariants);
#endif

	//fill input vectors
	for (ConfigParameter& input : yamlconfig.inputs) {
		data.messageInputList.push_back(convertToAnnotatedMessage(input));
	}
	//fill output vectors and internalState for temporary storage
	for (ConfigParameter& output : yamlconfig.outputs) {
		data.messageOutputList.push_back(convertToAnnotatedMessage(output));
	}
	return 0;
}

void Mapper::mapToInternalState(std::string& value, std::string& interfaceName) {
	for (AnnotatedMessage &annotatedMessage : data.messageOutputList)
	{
		if (annotatedMessage.interface_name == interfaceName) {
			annotatedMessage.message = value;
			return;
		}
	}
	std::cerr << __FUNCTION__ << " Could not map variable " << interfaceName << " of type "
	 << " to internal state because there is no variable of such name and type to map to." << std::endl;
	//Not found
	throw 404;
}

std::string Mapper::mapFromInternalState(std::string& interfaceName)
{
	for (AnnotatedMessage &annotatedMessage : data.messageInputList)
	{
		if (annotatedMessage.interface_name == interfaceName) {
			return annotatedMessage.message;
		}
	}
	std::cout << __FUNCTION__ << " Could not map variable " << interfaceName << " of type " << std::boolalpha
	 << " from internal state because there is no variable of such name and type to map from." << std::endl;
	//Not found
	throw 404;
}

void Mapper::setOwner(std::weak_ptr<SimulatorInterface> owner) {
	this->owner = owner;
}

