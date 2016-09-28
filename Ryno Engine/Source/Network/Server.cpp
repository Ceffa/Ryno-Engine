#include "Server.h"
#include "Log.h"


namespace Ryno {


	void Server::init() {
		
		server_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (server_sock == INVALID_SOCKET)
			die("failed to create server socket.");

		//Fill socket
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = inet_addr(SERVERIP);
		server_addr.sin_port = htons(SERVERPORT);
		print("server socket created.");

	}
	void Server::bind() {
		if (::bind(server_sock, (const sockaddr *)&server_addr, sizeof(server_addr)) != 0)
			die("bind failed.");
		print("bind success.");

	}
	void Server::listen() {
		if (::listen(server_sock, 1) != 0)
			die("listen failed.");
		print("listen success.");
		
	}
	void Server::accept() {
		print("Waiting for a connection");

		sockaddr_in client_addr;
		int addr_size = sizeof(client_addr);
		SOCKET clientSocket = ::accept(server_sock, (sockaddr *)&client_addr, &addr_size);
		if (clientSocket == INVALID_SOCKET)
			die("accept failed");				//TODO -> Recover 
		print("accept successful");
		
	}

	void Server::die(std::string s) {
		Log::println(s);
		server_sock = INVALID_SOCKET;
	}
	void Server::print(std::string s) {
		Log::println(s);
	}

}