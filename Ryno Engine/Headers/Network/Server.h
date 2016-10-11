#pragma once
#include "Socket.h"
#include "Connection.h"
#include <list>
namespace Ryno{

	class Server{
	public:
		Server::Server(C* _ip, U32 _port) : ip(_ip), port(_port) {}
		Server::~Server() { close(); }
		void init();
		void close();

		Socket sock;
		C* ip;
		U32 port;

	private:
		std::list<Connection*> conns;
		U32 max_conns = 100;

	};
}