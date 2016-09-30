#include "ClientSocket.h"

namespace Ryno {

	void ClientSocket::init() {

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
			die("failed to create socket.");

		//Address of server to connect
		addr.sin_family = AF_INET;
		addr.sin_port = htons(server_port);
		addr.sin_addr.s_addr = inet_addr(get_server_ip().c_str());
		print("Client socket created.");
	}

	void ClientSocket::close() {
		closesocket(sock);
		sock = INVALID_SOCKET;
		print("Server socket closed.");

	}

	void ClientSocket::connect() {
		if (::connect(sock, (const sockaddr *)&addr, sizeof addr) == SOCKET_ERROR)
			die("connect failed.");

		printf("connected to server.");
	}
	
}