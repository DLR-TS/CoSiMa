#include "SubProcessController.h"

void SubProcessController::spawnProcess(uint16_t& port) {
	const auto args = buildArgs(port);
#if defined(__linux__)
	runningProcesses.push_back(spawn(args));
#endif
}

void SubProcessController::shutdownProcesses() {
#if defined(__linux__)
	for (const auto& pid : runningProcesses) {
		kill(pid, SIGTERM);
		int status;
		waitpid(pid, &status, 0);
	}
	runningProcesses.clear();
#endif
}

std::vector<std::string> SubProcessController::buildArgs(uint16_t& port) {
	std::vector<std::string> args;
#if defined(__linux__)
	args.emplace_back("OSMPService");
#else
	args.emplace_back("OSMPService.exe");
#endif
	args.emplace_back(std::to_string(port));
	return args;
}

#if defined(__linux__)
pid_t SubProcessController::spawn(const std::vector<std::string>& args) {
	pid_t pid = fork();
	if (pid == -1) {
		// Fork failed
		return -1;
	}
	else if (pid == 0) {
		// Child process
		std::vector<char*> argv;
		for (const auto& arg : args) {
			argv.push_back(const_cast<char*>(arg.c_str()));
		}
		argv.push_back(nullptr); // Null-terminate the array
		execvp(argv[0], argv.data());
		// If execvp returns, it must have failed
		exit(EXIT_FAILURE);
	}
	else {
		// Parent process
		return pid;
	}
}

#endif
