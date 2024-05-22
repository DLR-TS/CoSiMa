#include "SubProcessController.h"

void SubProcessController::spawnProcess(SPC_EXECUTABLE executable, const uint16_t& port) {

#if defined(__linux__)
	std::vector<std::string> args;
	args.emplace_back("./OSMPService");
	args.emplace_back(std::to_string(port));
	runningProcesses.push_back(spawn(args));
#endif
#if defined(_WIN32)
	const std::string exe = executable == SPC_EXECUTABLE::CarlaOSISerivce ? "CARLA_OSI_Service.exe" : "OSMPService.exe";
	runningProcesses.push_back(spawn(exe, std::to_string(port)));
#endif
}

void SubProcessController::shutdownProcesses() {
#if defined(__linux__)
	for (const auto& pid : runningProcesses) {
		kill(pid, SIGTERM);
		int status;
		waitpid(pid, &status, 0);
	}
#endif
#if defined(_WIN32)
	for (const auto& process : runningProcesses) {
		TerminateProcess(process.hProcess, 0);
		CloseHandle(process.hProcess);
	}
#endif
	runningProcesses.clear();
}

#if defined(__linux__)
pid_t SubProcessController::spawn(const std::vector<std::string>& args) {
	pid_t pid = fork();
	if (pid == -1) {
		std::cerr << "Fork failed" << std::endl;
		// Fork failed
		return -1;
	}
	else if (pid == 0) {
	std::vector<char*> argv;
	for (const auto& arg : args) {
		argv.push_back(const_cast<char*>(arg.c_str()));
	}
	argv.push_back(nullptr); // Null-terminate the array
		// Child process
		execvp(argv[0], argv.data());
		exit(0);
	}
	else {
	//std::cout << "Child PID" << pid << std::endl;
	// Parent process
	}
	return pid;
}
#endif

#if defined(_WIN32)
// helper to widen a narrow UTF8 string in Win32
static const wchar_t* widen(const std::string& narrow, std::wstring& wide)
{
	size_t length = narrow.size();

	if (length > 0)
	{
		wide.resize(length);
		length = MultiByteToWideChar(CP_UTF8, 0, narrow.c_str(), (int)length, (wchar_t*)wide.c_str(), (int)length);
		wide.resize(length);
	}
	else
	{
		wide.clear();
	}

	return wide.c_str();
}

PROCESS_INFORMATION SubProcessController::spawn(const std::string& app, const std::string& arg) {
	// Prepare handles.
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	ZeroMemory(&pi, sizeof(pi));

	//Prepare CreateProcess args
	std::wstring app_w;
	widen(app, app_w);

	std::wstring arg_w;
	widen(arg, arg_w);

	std::wstring input = app_w + L" " + arg_w;
	wchar_t* arg_concat = const_cast<wchar_t*>(input.c_str());

	// Start the child process.
	if (!CreateProcessW(
		NULL,      // app path
		arg_concat,     // Command line (needs to include app path as first argument. args seperated by whitepace)
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi)            // Pointer to PROCESS_INFORMATION structure
		)
	{
		std::cerr << "CreateProcess failed " << GetLastError() << std::endl;
		throw std::exception("Could not create child process");
	}
	else
	{
		std::cout << "Successfully launched child process" << std::endl;
	}

	return pi;
}
#endif
