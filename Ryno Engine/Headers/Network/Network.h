#pragma once

#include "Client.h"
#include "Server.h"
#include "Global.h"
#include "NetUtil.h"
#include "Thread.h"

namespace Ryno{

	static class Network {
	public:
		void init(); 
		void create_client();
		void create_server();
		static Network* get_instance();

	private:
		Network::Network() {}
		Thread t;
		void threaded_create_client();
		void threaded_create_server();
	};

}