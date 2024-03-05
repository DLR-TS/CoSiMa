#include "ScenarioRunnerAdapter.h"

int ScenarioRunnerAdapter::init() {
#if defined(_WIN32)
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		return 1;
	}

	struct addrinfo* result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
#endif
#if defined(__linux__)

	struct sockaddr_in serverAddr, clientAddr;
	socklen_t addrLen = sizeof(clientAddr);

	// Create a TCP socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		perror("Error creating socket");
		return 1;
	}

	// Set up the server address struct
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port); // Port to listen on
	serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on any address

	// Bind the socket to the server address
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		perror("Error binding socket");
		close(serverSocket);
		return 1;
	}

	// Listen for incoming connections
	if (listen(serverSocket, 1) == -1) {
		perror("Error listening on socket");
		close(serverSocket);
		return 1;
	}

	// Accept a connection
	clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
	if (clientSocket == -1) {
		perror("Error accepting connection");
		close(serverSocket);
		return 1;
	}
#endif
	return 0;
}

double ScenarioRunnerAdapter::waitForTick() {

	double receivedDouble;
	// Receive double from client
#if defined(_WIN32)
	int iResult = recv(ClientSocket, reinterpret_cast<char*>(&receivedDouble), sizeof(double), 0);
	if (iResult == SOCKET_ERROR) {
		std::cout << "recv failed: " << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
#endif
#if defined(__linux__)
	if (recv(clientSocket, &receivedDouble, sizeof(receivedDouble), 0) == -1) {
		perror("Error receiving data");
		close(clientSocket);
		close(serverSocket);
		return 1;
	}
#endif
	return receivedDouble;
}

int ScenarioRunnerAdapter::sendTickDone(double timestamp) {
	 //std::cout << "Try to send tick to scenario runner back " << timestamp << std::endl;
#if defined(_WIN32)
	int iResult = send(ClientSocket, reinterpret_cast<char*>(&timestamp), sizeof(double), 0);
	if (iResult == SOCKET_ERROR) {
		std::cout << "send failed: " << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

#endif
#if defined(__linux__)

	if (send(clientSocket, &timestamp, sizeof(double), 0) == -1) {
		perror("Error sending data");
		close(clientSocket);
		close(serverSocket);
		return 1;
	}
#endif
	return 0;
}
