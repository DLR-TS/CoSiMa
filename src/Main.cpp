/**
@authors German Aerospace Center: Björn Bahn
*/

#include "Main.h"

int main(int argc, char *argv[])
{
	std::cout << "Welcome to CoSiMa.\n" << std::endl;

	CmdParameter runtimeParameter = parseRuntimeParameter(argc, argv);

	Cosima cosima;
	cosima.setRuntimeParameter(runtimeParameter);
	cosima.loadConfiguration();
	cosima.initInterfaces();
	cosima.sensorViewConfiguration();
	cosima.simulationLoop();

	std::cout << "Goodbye from CoSiMa." << std::endl;
	return 0;
}
