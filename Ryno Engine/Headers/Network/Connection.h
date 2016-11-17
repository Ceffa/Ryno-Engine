#pragma once
#include "Socket.h"
#include "NetStructs.h"

namespace Ryno{
	

	class Connection {
	public:
		static U32 last_client_id;
		U32 client_id;
		Socket* sock;
		SmallAddress address;

		Connection::Connection();
		Connection::Connection(Socket& _sock,const SmallAddress& _addr);
		Connection::~Connection();
		bool want_read();
		bool want_write();

		bool do_read(NetMessage* message);
		bool do_write(const NetMessage* message);

	};
}