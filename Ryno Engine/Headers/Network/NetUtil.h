#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Global.h"
#include <sstream>
#include <WinSock2.h>
#include <codecvt>
#include "Log.h"



namespace Ryno{

	
	struct Address : public sockaddr_in {
		Address() {}
		Address(const std::string& ip, const U16 port) {
			set(ip, port);
		}
		Address(const U32 ip, const U16 port)  {
			sin_addr.s_addr = ip;
			sin_port = port;
		}
		void set(const std::string& ip, const U16 port) {
			sin_family = AF_INET;
			sin_addr.s_addr = inet_addr(ip.c_str());
			sin_port = htons(port);
		}

		std::string to_string() const {
			std::string s = inet_ntoa(sin_addr);
			s += " : ";
			s += std::to_string(ntohs(sin_port));
			return s;
		}

		bool equals(const Address& other) const {
			return sin_addr.s_addr == other.sin_addr.s_addr && sin_port == other.sin_port;
		}
	};

	//small struct to pack ip and port in a datagram packet
	struct SmallAddress {
		SmallAddress() {}
		SmallAddress(const Address& address) { set(address); }
		void set(const Address& address) {
			ip = address.sin_addr.s_addr;
			port = address.sin_port;
		}
		bool equals(const SmallAddress& other)const {
			return ip == other.ip && port == other.port;
		}

		std::string to_string() const {
			return Address(ip, port).to_string();
		}
		U32 ip;
		U16 port;
	};


	static class NetUtil {
	
	public:

		static U32 compress_ip(std::string s);

		static std::string decompress_ip(U32 _ip);

		static std::string get_error();

		static void print(std::string s);
		static void print(I32 n);

		static sockaddr_in get_sockaddr(const std::string& ip , U32 port);

		static void print_error(std::string s);

	};
}