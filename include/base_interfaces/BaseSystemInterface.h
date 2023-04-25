/**
@authors German Aerospace Center: Björn Bahn, Nils Wendorff, Danny Behnecke
*/

#ifndef BASESYSTEMINTERFACE_H
#define BASESYSTEMINTERFACE_H

#include <string>
#include "reader/BaseConfigVariants.h"

class BaseSystemInterface
{
private:
	std::atomic<bool> simulationStop = false;
protected:
	bool verbose = false;
public:
	/**
	Read configuration for this base simulator interface.
	\param node yaml confguration node
	\return valid status
	*/
	virtual void configure(YAML::detail::iterator_value& node) = 0;
	/**
	Connect grpc with host/port information from corresponding fields
	\verbose enable verbose output
	\return success status
	*/
	virtual int init(bool verbose) = 0;
	/**
	Perform a simulation step
	\return Time in seconds advanced during step
	*/
	virtual double doStep(double stepSize = 1) = 0;

	/**
	Disconnect from base system interface
	\return Time in seconds advanced during step
	*/
	virtual int disconnect() = 0;

	virtual int getIntValue(std::string base_name) = 0;
	virtual bool getBoolValue(std::string base_name) = 0;
	virtual float getFloatValue(std::string base_name) = 0;
	virtual double getDoubleValue(std::string base_name) = 0;
	virtual std::string getStringValue(std::string base_name) = 0;

	virtual int setIntValue(std::string base_name, int value) = 0;
	virtual int setBoolValue(std::string base_name, bool value) = 0;
	virtual int setFloatValue(std::string base_name, float value) = 0;
	virtual int setDoubleValue(std::string base_name, double value) = 0;
	virtual int setStringValue(std::string base_name, std::string value) = 0;

	virtual double getStepSize() = 0;
	void stopSimulation() { simulationStop = true; };
	bool simulationStopped() { return simulationStop; };
};

#endif // !BASESYSTEMINTERFACE_H
