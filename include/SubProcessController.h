/**
@authors German Aerospace Center: Björn Bahn
*/

#ifndef SUBPROCESSSCONTROLLER_H
#define SUBPROCESSSCONTROLLER_H

#include <string>
#include <vector>
#include <iostream>

#if defined(_WIN32)
	#include <Windows.h>
	#include <processthreadsapi.h>
	#include <wchar.h>
#endif

enum SPC_EXECUTABLE {CarlaOSISerivce, OSMPService};

class SubProcessController {
public:
	void spawnProcess(const SPC_EXECUTABLE executable, const uint16_t& port);
	void shutdownProcesses();

private:

#if defined(__linux__)
	std::vector<pid_t> runningProcesses;
	pid_t spawn(const std::vector<std::string>& args);
#endif
#if defined(_WIN32)
	std::vector<PROCESS_INFORMATION> runningProcesses;
	PROCESS_INFORMATION spawn(const std::string& app, const std::string& arg);
#endif
};

#endif // !SUBPROCESSSCONTROLLER_H