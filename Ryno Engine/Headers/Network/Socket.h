#pragma once
#include "NetUtil.h"
#include "NetStructs.h"



namespace Ryno{
	class Network;

	struct State {
	private:
		I8 value = -1;
	public:
		const bool up() { return value == 1; }
		const bool down() { return value == -1; }
		const bool load() { return value == 0; }
		void set_up() { value = 1; }
		void set_down() { value = -1; }
		void set_loading() { value = 0; }
	};

	class Socket {
	private:
		SOCKET sock = SOCKET_ERROR;
	public:
		Socket::Socket();
		Socket::Socket(SOCKET _sock);

		const SOCKET get();
		bool init(bool datagram);
		void close();
		bool bind(SmallAddress& address);
		SmallAddress get_socket_address();
		I8 connect(const C* server_ip, U32 server_port);
		bool listen();
		Socket* accept();

		I32 send(const std::string* message);
		I8 recv_char(C* c);

		void set_blocking(bool b);

		State create_state;
		State bind_state;
		State listen_state;
		State connect_state;
		State accept_state;

		I32 send_struct(const NetMessage* message, const SmallAddress& to);
		I32 recv_struct(NetMessage* message, SmallAddress& from);

	private:

		void verify_socket();
		void reset_states();
	};

		





}