#pragma once

#include "NetEntity.h"
#include "Global.h"
#include "NetUtil.h"
#include "NetObject.h"


namespace Ryno{

	static class Network {
	public:
		void init(); 
		void start_client();
		void start_server();
		void update();
		static Network* get_instance();
		NetEntity* net_entity = nullptr;

	private:
		Network::Network() {}
		Network::~Network();

		void reset_entity();
	};

}