#include "NetObject.h"

namespace Ryno {
	std::set<NetObject*> NetObject::net_objects;



	bool NetObject::find(const NetId& id) {
		for (NetObject* obj : net_objects) {
			if (obj->id.equals(id))
				return true;
		}
		return false;
	}

}