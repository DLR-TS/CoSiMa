#include <iostream>
#include <cstring>

#if defined(_WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <exception>

#pragma comment(lib, "Ws2_32.lib")
#endif

#if defined(__linux__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

class ScenarioRunnerAdapter {
public:
	int init();
	double waitForTick();
	int sendTickDone(double timestamp);
	~ScenarioRunnerAdapter() {
#if defined(_WIN32)
		closesocket(ClientSocket);
		closesocket(ListenSocket);
		WSACleanup();
#endif
#if defined(__linux__)
		close(clientSocket);
		close(serverSocket);
#endif
	}

private:

	int port = 51424;

#if defined(_WIN32)
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket;
#endif
#if defined(__linux__)
	int serverSocket, clientSocket;
#endif
};
