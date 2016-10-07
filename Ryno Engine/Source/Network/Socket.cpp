#include "Socket.h"
#include "NetUtil.h"

namespace Ryno {

	Socket::Socket()
		: sock(INVALID_SOCKET), create_ok(false), bind_ok(false), listen_ok(false), connect_ok(false), accept_ok(false) {}
	Socket::Socket(SOCKET _sock) {
		sock = _sock;
		verify_socket();
	}


	const SOCKET Socket::get_handle() {
		return sock;
	}

	bool Socket::init() {
		sock = socket(AF_INET, SOCK_STREAM, 0);
		verify_socket();
		return create_ok;
	}

	void Socket::verify_socket() {
		if (sock == INVALID_SOCKET) {
			NetUtil::print_error("Socket create error: ");
			create_ok = false;
		}
		else {
			NetUtil::print("Socket create success: ");
			create_ok = true;
		}
	}

	bool Socket::send(Socket* receiver, const std::string* message) {
		char c = '\0';
		if (::send(receiver->get_handle(), message->c_str(), message->size(), 0) == SOCKET_ERROR
			|| ::send(receiver->get_handle(), &c, 1, 0) == SOCKET_ERROR)
		{
			NetUtil::print_error("Send error: ");
			return false;
		}
		return true;
	}

	bool Socket::recv(Socket* sender, std::string* message) {
		
		char c;
		while (true) {
			if (::recv(sender->get_handle(), &c, 1, 0) == SOCKET_ERROR) {
				NetUtil::print_error("Recv error: ");
				return false;
			}
			if (c == '\0')
				break;
			*message += c;
		}
		return true;
	}

	bool Socket::bind(const C* ip, U32 port) {
		if (!create_ok) {
			NetUtil::print("Cannot connect if socket not created.");
			return nullptr;
		}
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(port);
		if (::bind(sock, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
			bind_ok = false;
			NetUtil::print_error("Bind failed: ");
			return false;
		}
		bind_ok = true;
		NetUtil::print("Bind success.");
		return true;
	}

	bool Socket::connect(const C* server_ip, U32 server_port) {
		if (!create_ok) {
			NetUtil::print("Cannot connect if socket not created.");
			return nullptr;
		}
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(server_ip);
		addr.sin_port = htons(server_port);
		if (::connect(sock, (const sockaddr *)&addr, sizeof addr) == SOCKET_ERROR) {
			connect_ok = false;
			NetUtil::print_error("Connect failed: ");
			return false;
		}
		connect_ok = true;
		NetUtil::print("Connect success.");
		return true;
	}

	bool Socket::listen() {
		if (!bind_ok) {
			NetUtil::print("Cannot listen if not bound.");
			return nullptr;
		}
		if (::listen(sock, 1) != 0) {
			listen_ok = false;
			NetUtil::print_error("Listen failed: ");
			return false;
		}
		listen_ok = true;
		NetUtil::print("Listen success.");
		return true;
	}

	Socket* Socket::accept() {
		if (!listen_ok) {
			NetUtil::print("Cannot accept if not listening.");
			return nullptr;
		}
		sockaddr_in client_addr;
		int addr_size = sizeof(client_addr);
		Socket* client_sock = new Socket(::accept(sock, (sockaddr *)&client_addr, &addr_size));
		if (client_sock->get_handle() == INVALID_SOCKET) {
			accept_ok = false;
			NetUtil::print_error("Accept failed: ");
			delete client_sock;
			return nullptr;
		}
		accept_ok = true;
		NetUtil::print("Accept successful.");
		return client_sock;
	}


	void Socket::close() {
		create_ok = false;
		bind_ok = false;
		listen_ok = false;
		connect_ok = false;
		accept_ok = false;
		closesocket(sock);
		sock = INVALID_SOCKET;
	}


}