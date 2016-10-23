#pragma once

#include "Client.h"
#include "Server.h"
#include "Global.h"
#include "NetUtil.h"
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
		static Network* get_instance();
		Client* client = nullptr;
		Server* server = nullptr;
		bool has_server = false;
		bool has_client = false;

	private:
		Network::Network() {}
		Network::~Network();
	};

}