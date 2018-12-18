#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include "Header.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

SOCKET sock; // Makes the variable name sock accesible in the functions below
string name = "unknown"; // name is used as an input-variable for a name prompt later

void receiveMessage() {

	// Check for incoming messages
	while (true) {
		char buf[4096];
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived > 0) {
			// Echo response to console
			cout << string(buf, 0, bytesReceived) << endl;
		}
	}
}

void sendMessage() {
	while (true) {

		string userInput;

		// Promt the user for some text
		cout << "";
		getline(cin, userInput);

		// Send the text
		if (userInput.size() > 0) {
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		}
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
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create socket, Error #" << WSAGetLastError() << endl;
	}

	// Fill in the hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Prompt the user for her or his name
	cout << "Please enter your name:" << endl;
	cin >> name;

	// Send the entered name to the server
	if (name == "unknown") {
		cerr << "Name Prompt Failed... Quitting";
	}
	else {

		// Connect to server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR) { // if the client failed to connect to a server
			cerr << "Can't connect to server, Error #" << WSAGetLastError() << endl;
			closesocket(sock);
			WSACleanup();
			return;
		}
		else { // If the client is connected to a server
			cout << "Hi " + name + ". Welcome to the server!" << endl;

			// thread 1 starts running the send message function
			thread t1(sendMessage);

			// thread 1 detaches to continue running while thread 2 starts 
			t1.detach();

			// thread 2 starts running
			thread t2(receiveMessage);
			t2.join();
		}

		// Close down everything
		closesocket(sock);
		WSACleanup();
	}
}
