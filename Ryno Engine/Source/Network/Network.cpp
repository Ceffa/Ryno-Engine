#include "Network.h"
#include "Server.h"
#include "Client.h"


namespace Ryno {

	Client* Network::client = nullptr;
	Server* Network::server = nullptr;
	bool Network::has_client = false;
	bool Network::has_server = false;


	Network* Network::get_instance() {
		static Network instance;//only at the beginning
		return &instance;
	}

	Network::~Network() {
		reset();
	}

	void Network::init() {
		//Initialise win sock
		WSADATA w;
		int error = WSAStartup(0x0202, &w);
		if (error != 0)
			Log::println("WSAStartup failed");
	
		if (w.wVersion != 0x0202)
			Log::println("Wrong WinSock version");
	}
	
	void Network::start_server() {
		stop_server();	//just in case
		server = new Server("127.0.0.1", 5555);
		has_server = true;
		server->start();
		Log::print("server");
	}

	void Network::start_client() {
		stop_client();	//just in case
		client = new Client("127.0.0.1", 5555,"127.0.0.1");
		has_client = true;
		client->start();
		Log::print("client");

	}

	//Function that reset the network when the scene
	//is restarted
	void Network::reset() {
		stop_client();
		stop_server();
		NetId::last_id = 0;
		Connection::last_client_id = 0;
	}

	//Call update on server and/or client
	void Network::update() {
		if (has_server && server)
			if (!server->update())
				stop_server();
		if (has_client && client)
			if (!client->update())
				stop_client();
	}

	void Network::stop_client() {
		if (client) {
			delete client;
			client = nullptr;
		}
		has_client = false;
	}
	void Network::stop_server() {
		if (server) {
			delete server;
			server = nullptr;
		}
		has_server = false;
	}
}