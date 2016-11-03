#pragma once

#include "Client.h"
#include "Server.h"
#include "NetObject.h"


namespace Ryno{

	enum NetCode{POS_AND_COLOR,U_INT};

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
		Client* client = nullptr;
		Server* server = nullptr;
		bool has_server = false;
		bool has_client = false;

	private:
		Network::Network() {}
		Network::~Network();
	};

}