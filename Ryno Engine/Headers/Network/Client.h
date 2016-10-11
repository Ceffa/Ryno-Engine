#pragma once
#include "NetEntity.h"

namespace Ryno{

	class Client : public NetEntity{
	public:
		Client::Client(C* _server_ip, U32 _server_port) : NetEntity(_server_ip, _server_port){ }
		Client::~Client() { close(); }

		void start() override;
		void update() override;
		void close() override;
	
	};
}