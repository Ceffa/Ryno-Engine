#include "Network.h"

namespace Ryno {

	Network* Network::get_instance() {
		static Network instance;//only at the beginning
		return &instance;
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

	void Network::threaded_create_server() {

		Server server("127.0.0.1",5555);
		server.init();
	}

	void Network::threaded_create_client() {

		Client client("127.0.0.1", 5555);
		client.init();
	}
	
	void Network::create_server() {
		t.create(&Network::threaded_create_server, this);
	}

	void Network::create_client() {
		t.create(&Network::threaded_create_client, this);
	}
}