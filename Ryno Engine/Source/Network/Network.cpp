#include "Network.h"
#include "Server.h"
#include "Client.h"


namespace Ryno {

	Network* Network::get_instance() {
		static Network instance;//only at the beginning
		return &instance;
	}

	Network::~Network() {
		reset_entity();
	}

	void Network::init() {
		// Initialise the WinSock library -- we want version 2.2.
		WSADATA w;
		int error = WSAStartup(0x0202, &w);
		if (error != 0)
			Log::println("WSAStartup failed");
	
		if (w.wVersion != 0x0202)
			Log::println("Wrong WinSock version");
	}
	
	void Network::start_server() {
		reset_entity();
		net_entity = new Server("127.0.0.1", 5555);
		net_entity->start();
	}

	void Network::start_client() {
		reset_entity();
		net_entity = new Client("127.0.0.1", 0);
		net_entity->start();
	}

	void Network::update() {
		net_entity->update();
	}


	void Network::reset_entity() {
		if (net_entity) {
			delete net_entity;
			net_entity = nullptr;
		}
	}
}