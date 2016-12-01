#pragma once
#include "NetStructs.h"
#include "NetEntity.h"
#include "NetworkTime.h"


namespace Ryno{
	class NetworkScene;

	//Client class
	class Client : public NetEntity{
	public:
		Client(C* _server_ip, U16 _server_port,C* _local_ip, U16 _local_port = 0) : server_address(_server_ip, _server_port){
			local_address.set(_local_ip, _local_port);
			connected = false;
		}
		~Client() { close(); }

		//Address (and port) of the server
		SmallAddress server_address;
		
		void start() override;
		bool update() override;
		void close() override;	

		//Class that grants a shared idea of time between clients
		NetworkTime net_time;
		//Unique number provided by the server to identify the client.
		U32 client_id;
	private: 
		//Send the time to the server in order to connect and/or update the network time
		void send_client_update();
		//If not, the client doesn't send updates to the server (except connection requests)
		bool connected;
	};
}