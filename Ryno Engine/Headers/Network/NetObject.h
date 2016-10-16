#pragma once
#include "Script.h"
#include "NetUtil.h"

namespace Ryno{

	struct NetObjId {
		U32 ip;
		U32 port;
		U32 local_id;
		void to_network_order();
		void to_hardware_order();
	};
	class NetObject : public Script {
	public:
		NetObject() {}
		~NetObject() {}
		NetObject(const NetObject& cp) = delete;

		NetObjId id;

		void update() override {}
		void input() override {}
		void start() override {}
		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };
		
	};

}