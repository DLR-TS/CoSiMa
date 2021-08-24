#include "mapper/OSIMapper.h"

int OSIMapper::readConfiguration(configVariants_t configVariants) {

	std::cout << "Read Configuration of OSI Mapper" << std::endl;

	if (std::get_if<OSIInterfaceConfig>(&configVariants) == nullptr && std::get_if<OSMPInterfaceConfig>(&configVariants) == nullptr) {
		std::cout << "Called with wrong configuration variant!" << std::endl;
		return 1;
	}

	if (owner != nullptr) {
		owner->readConfiguration(configVariants);
	}

	if (std::get_if<OSIInterfaceConfig>(&configVariants) != nullptr) {
		OSIInterfaceConfig interfaceConfig = std::get<OSIInterfaceConfig>(configVariants);
		//overwrite prefix default value if set in configuration
		if (interfaceConfig.prefix.length() != 0) {
			this->prefix = interfaceConfig.prefix;
		}
		for (auto& input : interfaceConfig.inputs) {
			config.stringInputList.push_back(NamesAndIndex(prefix + input.base_name, input.interface_name, (int)state->strings.size()));
			state->strings.push_back(std::string(input.default_value));
		}
		for (auto& output : interfaceConfig.outputs) {
			config.stringOutputList.push_back(NamesAndIndex(prefix + output.base_name, output.interface_name, (int)state->strings.size()));
			state->strings.push_back(std::string(output.default_value));
		}
	}
	else if (std::get_if<OSMPInterfaceConfig>(&configVariants) != nullptr) {
		OSMPInterfaceConfig interfaceConfig = std::get<OSMPInterfaceConfig>(configVariants);
		//overwrite prefix default value if set in configuration
		if (interfaceConfig.prefix.length() != 0) {
			this->prefix = interfaceConfig.prefix;
		}
		for (auto& input : interfaceConfig.inputs) {
			config.stringInputList.push_back(NamesAndIndex(prefix + input.base_name, input.interface_name, (int)state->strings.size()));
			state->strings.push_back(std::string(input.default_value));
		}
		//increase index - not clear
		int inputsize = (int)state->strings.size();
		for (auto& output : interfaceConfig.outputs) {
			config.stringOutputList.push_back(NamesAndIndex(prefix + output.base_name, output.interface_name, (int)state->strings.size() - inputsize));
			state->strings.push_back(std::string(output.default_value));
		}
	}
	return 0;
}
