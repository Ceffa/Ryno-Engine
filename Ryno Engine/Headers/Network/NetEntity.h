#pragma once
#include "Socket.h"

namespace Ryno{
	class NetworkScene;
	class NetEntity{
	public:
		NetEntity::NetEntity() {}
		NetEntity::~NetEntity() {}
		
		virtual void start() = 0;
		virtual bool update() = 0;
		virtual void close() = 0;

		SmallAddress local_address;
		Socket sock;
	protected:
		NetworkScene* net_scene;
	};
}