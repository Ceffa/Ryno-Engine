#pragma once
#include "Script.h"
#include "NetUtil.h"

namespace Ryno{

	struct NetObjId {
		NetObjId(const SmallAddress& addr)
		{
			get_from_address(addr);
		}
		void get_from_address(const SmallAddress& addr)  {
			ip = addr.ip;
			port = addr.port;
			local_id = ++last_net_id;	//first id is 1, 0 is error
		}
		U32 ip;
		U32 port;
		U32 local_id;

		static U32 last_net_id;
	};
	class NetObject : public Script {
	public:

		NetObject(const SmallAddress& addr) : id(addr) {}
		~NetObject() {}
		NetObject(const NetObject& cp) = delete;

		NetObjId id;

		NetObject* clone() override { return nullptr; }
		void update() override {}
		void input() override {}
		void start() override {}
		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };
		
	};

}