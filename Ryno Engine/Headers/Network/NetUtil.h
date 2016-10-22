#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Global.h"
#include <sstream>
#include <WinSock2.h>
#include <codecvt>
#include "Log.h"



namespace Ryno{

	//Big structure to handle locally information about an address
	struct Address : public sockaddr_in {
		Address() {}
		Address(const std::string& ip, const U16 port) {
			set(ip, port);
		}
		Address(const U32 ip, const U16 port)  {
			sin_family = AF_INET;
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

	//Small structure that holds info about an address. Send this over network
	struct SmallAddress {
		SmallAddress() {}
		SmallAddress(const std::string& _ip, const U16 _port) : SmallAddress(Address(_ip,_port)) {}

		SmallAddress(const Address& address) { set(address); }
		void set(const Address& address) {
			ip = address.sin_addr.s_addr;
			port = address.sin_port;
		}
		void set(const std::string& _ip, const U16 _port) {
			set(Address(_ip, _port));
		}
		bool equals(const SmallAddress& other)const {
			return ip == other.ip && port == other.port;
		}

		std::string to_string() const {
			return Address(ip, port).to_string();
		}

		Address get_address() const {
			return Address(ip,port);
		}
		U32 ip;
		U16 port;

		static U16 last_id;
	};

	//Small struct to pack address and local id of a network object.
	//It fully identifies an object over a network
	struct NetId {
		NetId() {}
		NetId(const SmallAddress& address) { set(address); }
		void set(const SmallAddress& address) {
			addr = address;
			local_id = htons(++last_id);
		}
		bool equals(const NetId& other)const {
			return same_client(other) && local_id == other.local_id;
		}
		bool same_client(const NetId& other)const {
			return addr.equals(other.addr);
		}

		std::string to_string() const {
			return addr.to_string() + " : " + std::to_string(ntohs(local_id));
		}
		SmallAddress addr;
		U16 local_id;

		static U16 last_id;
	};

	//The message unit sent over the network. It contains at least a NetId
	struct Message {
		NetId id;
		U32 x, y, z;
		U32 color;
		void to_network_order() {
			x = htonl(x);
			y = htonl(y);
			z = htonl(z);
			color = htonl(color);
		}
		void to_hardware_order() {
			x = ntohl(x);
			y = ntohl(y);
			z = ntohl(z);
			color = htonl(color);
		}
	};

	static class NetUtil {
	
	public:

		static U32 compress_ip(std::string s);

		static std::string decompress_ip(U32 _ip);

		static std::string get_error();

		static void print(std::string s);
		static void print(I32 n);

		static void print_error(std::string s);
		static I32 error_limit;

	};
}