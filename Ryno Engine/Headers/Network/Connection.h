#pragma once
#include "Socket.h"
#include "NetStructs.h"

namespace Ryno{
	
	//Keep info about a UDP connection.
	//Used by the server to keep track of clients
	class Connection {
	public:
		//Last client connected
		static U32 last_client_id;
		//List of disconnected clients -> their ids could be reused
		static std::list<U32> unused_client_ids;

		U32 client_id;
		Socket* sock;
		//Address of the client
		SmallAddress address;

		Connection::Connection() = delete;
		Connection::Connection(Socket& _sock,const SmallAddress& _addr);
		Connection::~Connection();
	
		//Wrappers on socket recv and read
		bool do_read(NetMessage* message);
		bool do_write(const NetMessage* message);

	};
}