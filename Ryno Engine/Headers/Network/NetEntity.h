#pragma once
#include "Socket.h"

namespace Ryno{

	struct pos {
		C c;
	};

	class NetEntity{
	public:
		NetEntity::NetEntity(C* _ip, U32 _port) : server_ip(_ip), server_port(_port) {}
		NetEntity::~NetEntity() {}
		
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void close() = 0;

		Socket sock;
		C* server_ip;
		U32 server_port;
	};
}