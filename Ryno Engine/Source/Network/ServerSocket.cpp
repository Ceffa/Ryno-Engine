#include "ServerSocket.h"

namespace Ryno {
	void ServerSocket::init() {

		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == INVALID_SOCKET) {
			print_error("Socket error: ");
			close();
			return;
		}
		//Fill socket
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(get_server_ip().c_str());
		addr.sin_port = htons(server_port);
		print("Server socket created.");
		is_created = true;
	}

	bool ServerSocket::send(const std::string& message) {
		if (!is_accepted) {
			print("Cannot send without an established connection.");
			return false;
		}
		else {
			char c = '\0';
			if (::send(client_socket, message.c_str(), message.size(), 0) == SOCKET_ERROR
				|| ::send(client_socket, &c, 1, 0) == SOCKET_ERROR)
			{
				print_error("Send error: ");
				return false;
			}
		}
		return true;
	}

	bool ServerSocket::recv(std::string& message) {
		if (!is_accepted) {
			print("Cannot receive without an established connection.");
			return false;
		}
		else {
			char c;
			while (true) {
				if (::recv(client_socket, &c, 1, 0) == SOCKET_ERROR) {
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


	void ServerSocket::close() {
		closesocket(server_socket);
		server_socket = INVALID_SOCKET;
		print("Server socket closed.");
		is_created = false;
		is_bound = false;
		is_listening = false;
		is_accepted = false;
	}

	void ServerSocket::bind() {
		if (::bind(server_socket, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
			print_error("Bind failed: ");
			return;
		}
		print("Bind success.");
	}
	void ServerSocket::listen() {
		if (::listen(server_socket, 1) != 0) {
			print_error("Listen failed: ");
			return;
		}
		print("Listen success.");

	}
	void ServerSocket::accept() {
		print("Waiting for a connection...");

		int addr_size = sizeof(client_addr);
		client_socket = ::accept(server_socket, (sockaddr *)&client_addr, &addr_size);
		if (client_socket == INVALID_SOCKET){
			print_error("Accept failed: ");
			return;
		}				
		is_accepted = true;
		print("Accept successful.");
	}


	std::string ServerSocket::get_client_ip() {
		return std::string(inet_ntoa(addr.sin_addr));
	}

	U32 ServerSocket::get_client_port() {
		return ntohs(addr.sin_port);
	}
}