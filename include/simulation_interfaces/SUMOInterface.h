/**
@authors German Aerospace Center: Björn Bahn
*/

#ifndef SUMOINTERFACE_H
#define SUMOINTERFACE_H

//undefine max macro from Windows.h (WinDefs.h) for SUMO
#if defined(_WIN32)
	#undef max
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "libsumo/libsumo.h"
#include "simulation_interfaces/iSimulationData.h"
#include "mapper/SUMOMapper.h"

class SUMOInterface : public SimulatorInterface {
public:
	SUMOInterface() {
		mapper = std::make_shared<SUMOMapper>();
	};
	int init(bool verbose, std::string configurationPath) override;
	bool isAutostart(uint16_t& port) override { return false; };
	void setPort(uint16_t port) override {};
	int disconnect() override;

	int writeToInternalState() override;
	int readFromInternalState() override;
	int doStep(double stepSize = 1) override;
	void stopSimulation() override;

	void configure(const YAML::Node& node) override;

private:
	SUMOInterfaceConfig config;
};

#endif //!PROXYINTERFACE_H
