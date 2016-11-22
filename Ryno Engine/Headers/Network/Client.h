#pragma once
#include "NetStructs.h"
#include "NetEntity.h"
#include "NetworkTime.h"


namespace Ryno{
	class NetworkScene;
	class Client : public NetEntity{
	public:
		Client(C* _server_ip, U16 _server_port,C* _local_ip, U16 _local_port = 0) : server_address(_server_ip, _server_port){
			local_address.set(_local_ip, _local_port);
			connected = false;
		}
		~Client() { close(); }

		SmallAddress server_address;
		Game* game;
		void start() override;
		bool update() override;
		void close() override;	

		NetworkTime net_time;
		U32 client_id;
	private: 
		void send_client_update();
		NetworkScene* net_scene;
		bool connected;
	};
}