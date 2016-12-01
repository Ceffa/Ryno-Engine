#pragma once

#include "Client.h"
#include "Server.h"
#include "NetObject.h"


namespace Ryno{

	//The main network class, which manages everything related to the network
	static class Network {
	public:

		//Life cycle
		void init(); 
		void update();
		void reset();

		//Client
		static bool has_client;
		static Client* client;
		void start_client();
		void stop_client();

		//Server
		static bool has_server;
		static Server* server;
		void start_server();
		void stop_server();
		static Network* get_instance();
	private:
		Network::Network() {}
		Network::~Network();
	};

}