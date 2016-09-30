#pragma once
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Global.h"
#include <winsock2.h>
#include <iostream>
#include "Thread.h"


#pragma comment(lib, "ws2_32.lib")


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