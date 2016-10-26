#pragma once
#include "Script.h"
#include "NetUtil.h"

namespace Ryno{

	class NetObject : public Script {
		friend class Client;
	public:
		NetObject(const NetId& addr) : id(addr), last_send(0) { net_objects.insert(this); }
		~NetObject() { net_objects.erase(this);  }
		NetObject(const NetObject& cp) = delete;

		NetId id;
		static std::set<NetObject*> net_objects;
		static NetObject* find(const NetId& id);
		NetObject* clone() override { return nullptr; }
		void update() override {}
		void input() override {}
		void start() override {}
		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };
	private:
		F32 last_send = 0;
	};

}