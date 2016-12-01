#pragma once
#include "Socket.h"

namespace Ryno{
	class NetworkScene;

	//Abstract class that must be inherited by client and server.
	class NetEntity{
	public:
		NetEntity::NetEntity() {}
		NetEntity::~NetEntity() {}
		
		//Hook methods called by the network class
		virtual void start() = 0;
		virtual bool update() = 0;
		virtual void close() = 0;

		//A compact representation of a winsock sockaddess
		SmallAddress local_address;
		//Socket handler
		Socket sock;
	protected:
		//Pointer to the current network scene being played
		NetworkScene* net_scene;
	};
}