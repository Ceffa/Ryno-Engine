#pragma once
#include "NetEntity.h"

namespace Ryno{

	class Client : public NetEntity{
	public:
		Client(C* _server_ip, U16 _server_port,C* _local_ip, U16 _local_port = 0) : server_address(_server_ip, _server_port), delay(.001f){
			local_address.set(_local_ip, _local_port);
		}
		~Client() { close(); }

		SmallAddress server_address;
		Game* game;
		void start() override;
		void update() override;
		void close() override;	

	private:
		F32 delay;
	};

}