/**
@authors German Aerospace Center: Björn Bahn
*/

#ifndef SUBPROCESSSCONTROLLER_H
#define SUBPROCESSSCONTROLLER_H

#include <string>
#include <vector>

class SubProcessController {
public:
	void spawnProcess(uint16_t& port);
	void shutdownProcesses();

private:
	std::vector<std::string> buildArgs(uint16_t& port);

#if defined(__linux__)
	std::vector<pid_t> runningProcesses;
	pid_t spawn(const std::vector<std::string>& args);
#endif

};

#endif // !SUBPROCESSSCONTROLLER_H