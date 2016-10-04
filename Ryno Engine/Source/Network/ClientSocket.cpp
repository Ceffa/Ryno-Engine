#include "ClientSocket.h"

namespace Ryno {

	void ClientSocket::init() {

		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == INVALID_SOCKET) {
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
	bool ClientSocket::send(const std::string& message) {
		if (!is_connected) {
			print("Cannot send without an established connection.");
			return false;
		}
		else {
			char c = '\0';
			//Send both the message and an empty byte
			if (::send(server_socket, message.c_str(), message.size(), 0) == SOCKET_ERROR
				|| ::send(server_socket, &c, 1, 0) == SOCKET_ERROR)
			{
				print_error("Send error: ");
				return false;
			}
		}
		return true;
	}

	bool ClientSocket::recv(std::string& message) {
		if (!is_connected) {
			print("Cannot receive without an established connection.");
			return false;
		}
		else {
			char c;
			while (true) {
				if (::recv(server_socket, &c, 1, 0) == SOCKET_ERROR) {
					print_error("Send error: ");
					return false;
				}
				if (c == '\0')
					break;
				message += c;
			}
		}
		return true;
	}


	void ClientSocket::close() {
		closesocket(server_socket);
		
		server_socket = INVALID_SOCKET;
		print("Server socket closed.");
		is_created = false;
		is_connected = false;
	}

	void ClientSocket::connect() {
		if (::connect(server_socket, (const sockaddr *)&addr, sizeof addr) == SOCKET_ERROR) {
			print_error("Connect failed: ");
			is_connected = false;
			return;
		}
		print("Connect success.");
		is_connected = true;
	}
	
}