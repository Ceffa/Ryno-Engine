#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "ws2_32.lib")
#include "Global.h"
#include <WinSock2.h>


namespace Ryno{

	//Big structure to handle locally information about an address
	struct Address : public sockaddr_in {
		Address() {}
		Address(const std::string& ip, const U16 port) {
			set(ip, port);
		}
		Address(const U32 ip, const U16 port) {
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
		SmallAddress(const std::string& _ip, const U16 _port) : SmallAddress(Address(_ip, _port)) {}

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
			return Address(ip, port);
		}
		U32 ip;
		U16 port;

		static U16 last_id;
	};

	//Small struct to pack address and local id of a network object.
	//It fully identifies an object over a network
	struct NetId {
		NetId() {}
		NetId(U32 _client_id) { set(_client_id); }
		void set(U32 _client_id) {
			client_id = _client_id;
			object_id = ++last_id;
		}
		bool equals(const NetId& other)const {
			return same_client(other) && object_id == other.object_id;
		}
		bool same_client(const NetId& other)const {
			return client_id == other.client_id;
		}

		U16 client_id, object_id;

		static U16 last_id;
	};

	//Base network struct that supports 
	//conversion to and from network order
	class NetStruct {
	public:
		NetStruct() {}
		virtual void to_network_order() = 0;
		virtual void to_hardware_order() = 0;
		template <class To, class From>
		static To convert(From f) {
			return *(To*)&f;
		}
	};

	//Net struct that contains mandatory 
	//information about a packet
	class Header : public NetStruct {
	public:
		Header() {}
		Header(U32 _code, const NetId& _id) : code(_code), id(_id){}
		U32 code;
		NetId id;
		void to_network_order() override {
			code = htonl(code);
			id.object_id = htons(id.object_id);
			id.client_id = htons(id.client_id);
		}
		void to_hardware_order() override {
			code = ntohl(code);
			id.object_id = ntohs(id.object_id);
			id.client_id = ntohs(id.client_id);
		}
	};

	//Sent by client periodically
	class ClientUpdate : public NetStruct {
	public:
		void to_network_order() override {
		}
		void to_hardware_order() override {
		}
	};
	class ServerUpdate : public NetStruct {
	public:
		U32 client_id;
		void to_network_order() override {
			net_time = htonl(net_time);
			client_id = htonl(client_id);
		}
		void to_hardware_order() override {
			net_time = ntohl(net_time);
			client_id = ntohl(client_id);
		}
		void set_time(F32 t) {
			net_time = NetStruct::convert<U32>(t);
		}
		F32 get_time() {
			return NetStruct::convert<F32>(net_time);
		}

	private:
		U32 net_time;
	};

	class ObjectUpdate : public NetStruct {
	public:
		ObjectUpdate() {}
		ObjectUpdate(U32 _code): code(_code) {}
		U32 code;
		
		void to_network_order() override {
			code = htonl(code);
			x = htonl(x);
			y = htonl(y);
			z = htonl(z);
			pitch = htonl(pitch);
			yaw = htonl(yaw);
			roll = htonl(roll);
			w = htonl(w);
			h = htonl(h);
			d = htonl(d);
		}
		void to_hardware_order() override {
			code = ntohl(code);
			x = ntohl(x);
			y = ntohl(y);
			z = ntohl(z);
			pitch = ntohl(pitch);
			yaw = ntohl(yaw);
			roll = ntohl(roll);
			w = ntohl(w);
			h = ntohl(h);
			d = ntohl(d);
		}

		void set_pos(glm::vec3 pos) {
			x = NetStruct::convert<U32>(pos.x);
			y = NetStruct::convert<U32>(pos.y);
			z = NetStruct::convert<U32>(pos.z);
		}
		void set_rot(glm::vec3 rot) {
			pitch = NetStruct::convert<U32>(rot.x);
			yaw = NetStruct::convert<U32>(rot.y);
			roll = NetStruct::convert<U32>(rot.z);
		}
		void set_scale(glm::vec3 scale) {
			w = NetStruct::convert<U32>(scale.x);
			h = NetStruct::convert<U32>(scale.y);
			d = NetStruct::convert<U32>(scale.z);
		}
		glm::vec3 get_pos() const {
			glm::vec3 pos;
			pos.x = NetStruct::convert<F32>(x);
			pos.y = NetStruct::convert<F32>(y);
			pos.z = NetStruct::convert<F32>(z);
			return pos;
		}
		glm::vec3 get_rot()const {
			glm::vec3 rot;
			rot.x = NetStruct::convert<F32>(pitch);
			rot.y = NetStruct::convert<F32>(yaw);
			rot.z = NetStruct::convert<F32>(roll);
			return rot;
		}
		glm::vec3 get_scale() const {
			glm::vec3 scale;
			scale.x = NetStruct::convert<F32>(w);
			scale.y = NetStruct::convert<F32>(h);
			scale.z = NetStruct::convert<F32>(d);
			return scale;
		}

	private:
		U32 x, y, z;
		U32 pitch, yaw, roll;
		U32 w, h, d;
	};

	enum NetCode { CLIENT_UPDATE,SERVER_UPDATE, OBJECT };

	struct NetMessage {
		NetMessage() {}
		Header header;
		union {
			ClientUpdate client_update;
			ServerUpdate server_update;
			ObjectUpdate object_update;
		};

	};


}