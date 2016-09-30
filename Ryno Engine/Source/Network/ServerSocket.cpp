#include "ServerSocket.h"

namespace Ryno {
	void ServerSocket::init() {

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
			die("failed to create server socket.");

		//Fill socket
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(get_server_ip().c_str());
		addr.sin_port = htons(server_port);
		print("server socket created.");

	}

	void ServerSocket::close() {
		closesocket(sock);
		sock = INVALID_SOCKET;
		print("Server socket closed.");

	}

	void ServerSocket::bind() {
		if (::bind(sock, (const sockaddr *)&addr, sizeof(addr)) != 0)
			die("bind failed.");
		print("bind success.");

	}
	void ServerSocket::listen() {
		if (::listen(sock, 1) != 0)
			die("listen failed.");
		print("listen success.");

	}
	void ServerSocket::accept() {
		print("Waiting for a connection");

		int addr_size = sizeof(client_addr);
		SOCKET clientSocket = ::accept(sock, (sockaddr *)&client_addr, &addr_size);
		if (clientSocket == INVALID_SOCKET)
			die("accept failed");				//TODO -> Recover 
		print("accept successful");
	}


	std::string ServerSocket::get_client_ip() {
		return std::string(inet_ntoa(addr.sin_addr));
	}

	U32 ServerSocket::get_client_port() {
		return ntohs(addr.sin_port);
	}
}