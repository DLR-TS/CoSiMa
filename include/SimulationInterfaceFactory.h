/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef SIMULATIONINTERFACEFACTORY_H
#define SIMULATIONINTERFACEFACTORY_H

#include <string>
//simulation interfaces
#include "simulation_interfaces/iSimulationData.h"
#include "simulation_interfaces/FMIBridge.h"
#include "simulation_interfaces/OSIBridge.h"
#include "simulation_interfaces/DefaultBridge.h"
#include "simulation_interfaces/OSMPBridge.h"
#include "simulation_interfaces/OSMPInterface.h"
//Mapper
#include "mapper/Mapper.h"
#include "mapper/FMIMapper.h"
#include "mapper/OSIMapper.h"
#include "mapper/DefaultMapper.h"

/**
* Factory of interface objects
*/
class SimulationInterfaceFactory {
public:
	/**
	Create interface object of given simulator interface with default owner configuration.
	\param simulator simulator type to create
	\return simulator interface
	*/
	static std::shared_ptr<iSimulationData> makeInterface(eSimulatorName simulator, bool debug);

private:
	/**
	Create interface object of given simulator interface.
	\param simulator simulator type to create
	\return simulator interface
	*/
	static std::unique_ptr<iSimulationData> createInterface(eSimulatorName simulator, bool debug);
};

#endif // !SIMULATIONINTERFACEFACTORY_H
