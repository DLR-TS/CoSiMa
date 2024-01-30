#include "mapper/OSIMapper.h"

int OSIMapper::readConfiguration(configVariants_t configVariants) {

	std::cout << "Read Configuration of OSI Mapper" << std::endl;

#if __has_include(<variant>)
	OSMPInterfaceConfig interfaceConfig = std::get<OSMPInterfaceConfig>(configVariants);
#elif __has_include("boost/variant.hpp")
	OSMPInterfaceConfig interfaceConfig = boost::get<OSMPInterfaceConfig>(configVariants);
#endif
	//fill input vectors
	for (auto& input : interfaceConfig.inputs) {
		data.messageInputList.push_back(convertToAnnotatedMessage(input));
	}
	//fill output vectors and internalState for temporary storage
	for (auto& output : interfaceConfig.outputs) {
		data.messageOutputList.push_back(convertToAnnotatedMessage(output));
	}
	return 0;
}
