#pragma once
#include "NetEntity.h"
#include "Connection.h"
#include <set>
namespace Ryno{

	//Server class
	class Server : public NetEntity{
	public:
		Server::Server(C* _ip, U32 _port) { local_address.set(_ip, _port); last_periodic_update = -999; update_frequence = .5f; }
		Server::~Server() { close(); }
		void start() override;
		bool update() override;
		void close() override;

		void request_update() { last_periodic_update = -999; }

	private:

		//Handle connections
		std::list<Connection*> conns;
		U32 max_conns = 100;
		Connection* add_to_connections(const SmallAddress& addr);
		void remove_from_connections(const SmallAddress& addr);
		Connection* find_connection(const SmallAddress& addr);

		//Updates
		F32 last_periodic_update;
		F32 update_frequence;
		
		//Needed for select
		timeval timeout;
		fd_set readable;
	};
}