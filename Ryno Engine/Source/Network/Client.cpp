#include "Client.h"
#include "Log.h"
 


namespace Ryno {

	void Client::init() {
		
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
			die("failed to create socket.");

		//Address of server to connect
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(SERVERPORT);
		server_addr.sin_addr.s_addr = inet_addr(SERVERIP);
		print("client socket created.");


	}
	void Client::connect() {
		if (::connect(sock, (const sockaddr *)&server_addr, sizeof server_addr) == SOCKET_ERROR)
			die("connect failed.");
	
		printf("connected to server.");
	}

	void Client::die(std::string s) {
		Log::println(s);
		sock = INVALID_SOCKET;
	}
	void Client::print(std::string s) {
		Log::println(s);
	}

}