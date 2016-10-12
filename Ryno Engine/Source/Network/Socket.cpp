#include "Socket.h"
#include "NetUtil.h"

namespace Ryno {

	Socket::Socket()
		: sock(INVALID_SOCKET) {
		reset_states();
	}

	Socket::Socket(SOCKET _sock) {
		sock = _sock;
		verify_socket();
	}


	const SOCKET Socket::get() {
		return sock;
	}

	bool Socket::init() {
		sock = socket(AF_INET, SOCK_STREAM, 0);
		verify_socket();
		return create_state.up();
	}

	void Socket::verify_socket() {
		if (sock == INVALID_SOCKET) {
			NetUtil::print_error("Socket create error: ");
			create_state.set_down();
		}
		else {
			NetUtil::print("Socket create success.");
			create_state.set_up();
		}
	}

	I32 Socket::send(const std::string* message) {
		I32 size = ::send(sock, message->c_str(),message->size()+1, 0);
		if (size == SOCKET_ERROR)
		{
			I32 error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK) {
				return 0;
			}
			NetUtil::print_error("Send error: ");
			return -1;
		}

		return size;
	}

	I8 Socket::recv_char(C* c) {
		
		if (::recv(sock, c, 1, 0) == SOCKET_ERROR) {
			I32 error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK) {
				return 0;
			}
			else {
				NetUtil::print_error("Recv error: ");
				return -1;
			}
		}
		return 1;
	}

	bool Socket::bind(const C* ip, U32 port) {
		if (!create_state.up()) {
			NetUtil::print("Cannot connect if socket not created.");
			return nullptr;
		}
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(port);
		if (::bind(sock, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
			bind_state.set_down();
			NetUtil::print_error("Bind failed: ");
			return false;
		}
		bind_state.set_up();
		NetUtil::print("Bind success.");
		return true;
	}

	I8 Socket::connect(const C* server_ip, U32 server_port) {
		if (!create_state.up()) {
			NetUtil::print("Cannot connect if socket not created.");
			return -1;
		}
		//Already connected
		if (connect_state.up())
			return 1;
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(server_ip);
		addr.sin_port = htons(server_port);
		
		if (::connect(sock, (const sockaddr *)&addr, sizeof addr) == SOCKET_ERROR) {
			
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				connect_state.set_loading();
				NetUtil::print_error("Connect loading: ");
				return 0;
			}
			else {
				connect_state.set_down();
				NetUtil::print_error("Connect failed: ");
				return -1;
			}
		}
		connect_state.set_up();
		NetUtil::print("Connect success.");
		return 1;
	}

	bool Socket::listen() {
		if (!bind_state.up()) {
			NetUtil::print("Cannot listen if not bound.");
			return nullptr;
		}
		if (::listen(sock, 1) != 0) {
			listen_state.set_down();
			NetUtil::print_error("Listen failed: ");
			return false;
		}
		listen_state.set_up();
		NetUtil::print("Listen success.");
		return true;
	}

	Socket* Socket::accept() {
		if (!listen_state.up()) {
			NetUtil::print("Cannot accept if not listening.");
			return nullptr;
		}
		sockaddr_in client_addr;
		int addr_size = sizeof(client_addr);
		Socket* client_sock = new Socket(::accept(sock, (sockaddr *)&client_addr, &addr_size));
		if (client_sock->get() == INVALID_SOCKET) {
			accept_state.down();
			NetUtil::print_error("Accept failed: ");
			delete client_sock;
			return nullptr;
		}
		accept_state.up();
		NetUtil::print("Accept successful.");
		return client_sock;
	}


	void Socket::close() {
		closesocket(sock);
		sock = INVALID_SOCKET;
	}

	void Socket::set_blocking(bool b) {
		ULONG value = b ? 0 : 1;
		ioctlsocket(sock, FIONBIO, &value);
	}

	void Socket::reset_states() {
		create_state.set_down();
		bind_state.set_down();
		listen_state.set_down();
		connect_state.set_down();
		accept_state.set_down();
	}
}