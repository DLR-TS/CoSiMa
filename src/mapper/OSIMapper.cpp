#include "mapper/OSIMapper.h"

int OSIMapper::readConfiguration(configVariants_t configVariants) {

	std::cout << "Read Configuration of OSI Mapper" << std::endl;

	if (std::get_if<OSMPInterfaceConfig>(&configVariants) == nullptr) {
		std::cout << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	if (std::get_if<OSMPInterfaceConfig>(&configVariants) != nullptr) {
		OSMPInterfaceConfig interfaceConfig = std::get<OSMPInterfaceConfig>(configVariants);
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
	return -1;
}
