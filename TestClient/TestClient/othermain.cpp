#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void listener() {
	char buf[4096];

	// Check for incoming messages
	ZeroMemory(buf, 4096);
	int bytesReceived = recv(sock, buf, 4096, 0);
	if (bytesReceived > 0) {
		// Echo response to console
		cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
	}
}

void sendMessage() {
	// Promt the user for some text
	cout << "> ";
	getline(cin, userInput);

	// Send the text
	if (userInput.size() > 0) {
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
	}
}

void main()
{	
	string ipAddress = "127.0.0.1";		// Local IP address for the server
	int port = 54000;					// Listening port # on the server

	// Intialize WinSock, which is an API that allows for communication between Windows network software and network services
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Can't start Winsock, Error #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create socket, Error #" << WSAGetLastError() << endl;
	}

	// Fill in the hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, Error #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	thread t1(listener); // thread 1 starts running
	t1.detach();
	thread t2(sendMessage); // thread 2 starts running
	t2.join();

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
