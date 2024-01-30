#include "mapper/OSIMapper.h"

int OSIMapper::readConfiguration(configVariants_t configVariants) {

	std::cout << "Read Configuration of OSI Mapper" << std::endl;

#if defined(_WIN32) && (_MSC_VER >= 1910) || defined(__linux__) && __cplusplus >= 201703L
	OSMPInterfaceConfig interfaceConfig = std::get<OSMPInterfaceConfig>(configVariants);
#elif defined(_WIN32) && (_MSC_VER >= 1600) || defined(__linux__) && __cplusplus >= 201103L
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
