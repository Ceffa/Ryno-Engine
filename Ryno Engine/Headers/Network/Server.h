#pragma once
#include "NetEntity.h"
#include "Connection.h"
#include <set>
namespace Ryno{


	class Server : public NetEntity{
	public:
		Server::Server(C* _ip, U32 _port) { local_address.set(_ip, _port); last_periodic_update = -999; update_frequence = 1; }
		Server::~Server() { close(); }
		void start() override;
		bool update() override;
		void close() override;
		void set_timeout(U32 microseconds);
		F32 last_periodic_update;

	private:
		std::list<Connection*> conns;
		F32 update_frequence;
		U32 max_conns = 100;
		timeval timeout;
		fd_set readable;

		Connection* add_to_connections(const SmallAddress& addr);
		void remove_from_connections(const SmallAddress& addr);
		Connection* find_connection(const SmallAddress& addr);
	};
}