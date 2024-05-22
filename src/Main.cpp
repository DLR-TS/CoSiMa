/**
@authors German Aerospace Center: Björn Bahn
*/

#include "Main.h"

int main(int argc, char *argv[])
{
	std::cout << "Welcome to CoSiMa.\n" << std::endl;

	Cosima cosima;
	cosima.parseRuntimeParameter(argc, argv);
	cosima.loadConfiguration();
	cosima.spawnLocalServices();
	cosima.waitForActiveScenarioRunner();
	cosima.initInterfaces();
	cosima.sensorViewConfiguration();
	cosima.simulationLoop();
	cosima.stopLocalServices();

	std::cout << "Goodbye from CoSiMa." << std::endl;
	return 0;
}
