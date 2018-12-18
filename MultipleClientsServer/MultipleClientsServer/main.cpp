#include <iostream>
#include <WS2tcpip.h>
#include <sstream>

#pragma comment (lib, "WS2_32.lib")

using namespace std;

void main() {

	// intialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't intialize winsock... Quitting!" << endl;
		return;
	}

	// create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "Can't create socket... Quitting!" << endl;
		return;
	}

	// bind the socket to an ip adress and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// tell winsock that the socket is for listening
	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);

	FD_SET(listening, &master);

	while (true) {
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == listening) {
				// Accept a new connection
				SOCKET clientSock = accept(listening, nullptr, nullptr);

				// Print the IP of the client and the Port it is connected to in the server console
				sockaddr_in client;
				char host[NI_MAXHOST];	
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);

				cout << host << " connected on port " << ntohs(client.sin_port) << endl;

				// Add the new connection to the list of connected clients
				FD_SET(clientSock, &master);

			}
			else {
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0) {
					// Drop the client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else {
					// Send message to other clients, and definiately NOT the listening socket.

					for (int i = 0; i < master.fd_count; i++) {
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock) {
							
							ostringstream ss;
							ss << "User " << sock << ": " << buf;
							string strOut = ss.str();
							
							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
				}
			}
		}
	}

	// clean up winsock
	WSACleanup();

	system("pause");
}
