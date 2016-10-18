#pragma once
#include "Socket.h"

namespace Ryno{

	class NetEntity{
	public:
		NetEntity::NetEntity() {}
		NetEntity::~NetEntity() {}
		
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void close() = 0;

		SmallAddress local_address;
		Socket sock;
	};
}