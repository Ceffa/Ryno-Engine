#pragma once

#include "Client.h"
#include "Server.h"
#include "NetObject.h"


namespace Ryno{


	static class Network {
	public:
		void init(); 
		void start_client();
		void start_server();
		void stop_client();
		void stop_server();
		void update();
		void reset();
		static Network* get_instance();
		static Client* client;
		static Server* server;
		static bool has_server;
		static bool has_client;
	private:
		Network::Network() {}
		Network::~Network();
	};

}