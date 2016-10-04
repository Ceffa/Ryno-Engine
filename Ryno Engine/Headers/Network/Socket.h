#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma once
#include "Global.h"
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

namespace Ryno{

	class Socket {
	public:
		SOCKET server_socket = INVALID_SOCKET;

		bool is_created = false;

		virtual void init() = 0;
		virtual void close() = 0;
	
		void set_server_port(U32 _server_port);
		U32 get_server_port();
		std::string get_server_ip();
		void set_server_ip(std::string s);

		void print(std::string s);
		void print_error(std::string s);

		virtual bool send(const std::string& s) = 0;
		virtual bool recv(std::string& message) =0;

		



	protected:
		U32 server_ip;
		U32 server_port;
		sockaddr_in addr;

		std::string decompress_ip(U32 _ip);
		U32 compress_ip(std::string s);

		std::string get_error();

	};




}