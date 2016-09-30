#include "ClientSocket.h"

namespace Ryno {

	void ClientSocket::init() {

		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			print_error("Socket error: ");
			close();
			return;
		}
		//Address of server to connect
		addr.sin_family = AF_INET;
		addr.sin_port = htons(server_port);
		addr.sin_addr.s_addr = inet_addr(get_server_ip().c_str());
		print("Client socket created.");
		is_created = true;
	}

	void ClientSocket::close() {
		closesocket(sock);
		
		sock = INVALID_SOCKET;
		print("Server socket closed.");
		is_created = false;
		is_connected = false;
	}

	void ClientSocket::connect() {
		if (::connect(sock, (const sockaddr *)&addr, sizeof addr) == SOCKET_ERROR) {
			print_error("Connect failed: ");
			is_connected = false;
			return;
		}
		print("Connect success.");
		is_connected = true;
	}
	
}