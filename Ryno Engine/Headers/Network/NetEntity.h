#pragma once
#include "Socket.h"

namespace Ryno{

	struct pos {
		F32 x,y,z,_pad;
		std::string to_string() {
			std::string s;
			s += x;
			s += " ";
			s += y;
			s += " ";
			s += z;
			return s;
		}
	};

	class NetEntity{
	public:
		NetEntity::NetEntity(C* _ip, U32 _port) : local_address(_ip, _port) {}
	
		NetEntity::~NetEntity() {}
		
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void close() = 0;

		Address local_address;
		Socket sock;
	};
}