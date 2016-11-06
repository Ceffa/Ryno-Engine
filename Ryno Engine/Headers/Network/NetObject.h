#pragma once
#include "Script.h"
#include "NetStructs.h"
#include <set>

namespace Ryno{

	class NetObject : public Script {
		friend class Client;
		friend class NetworkScene;
	public:
		NetObject(const NetId& addr) : id(addr), last_update(0) { net_objects.insert(this); }
		~NetObject() { net_objects.erase(this);  }
		NetObject(const NetObject& cp) = delete;

		NetId id;
		static std::set<NetObject*> net_objects;
		static NetObject* find(const NetId& id);
		NetObject* clone() override { return nullptr; }
		void update() override;
		void input() override {}
		void start() override {}
		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };
		F32 last_modified;		//needed to desync the object if inactive
	private:
		F32 last_update = 0;	//needed to send messages in regular time intervals
		bool mark_for_destruction = false;
		static F32 disconnect_delay;
		static F32 send_delay;
	};

}