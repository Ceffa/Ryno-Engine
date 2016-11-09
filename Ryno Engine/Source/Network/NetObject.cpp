#include "NetObject.h"
#include "NetUtil.h"
#include "Network.h"
#include "Game.h"

namespace Ryno {
	std::set<NetObject*> NetObject::net_objects;
	Ryno::F32 NetObject::disconnect_delay = 2;
	Ryno::F32 NetObject::send_delay = .1f;

	NetObject::NetObject(const NetId& addr) : id(addr), last_update(0) {
		net_objects.insert(this);
		Client* c = Network::get_instance()->client;
		owned = id.addr.equals(c->local_address);
	}

	NetObject* NetObject::find(const NetId& id) {
		for (NetObject* obj : net_objects) {
			if (obj->id.equals(id))
				return obj;
		}
		return nullptr;
	}

	void NetObject::set_network_position(const glm::vec3& new_pos) {
		time_cache.new_position(new_pos);//probably add lerp here between estimated and actual pos
		time_cache.recalculate();
	}


	void TimeCache::recalculate() {
		calculate_times();
		calculate_velocities();
		calcultate_accelerations();
		calculate_jerk();
	}

	void NetObject::update()
	{
		if (!owned) {
			glm::vec3 new_pos = time_cache.pos[0] +(Game::get_instance()->time - time_cache.times[0]) * time_cache.vel[0];
			game_object->transform.set_position(new_pos);
		}
	}
	void TimeCache::calculate_times() {
		times[1] = times[0];
		times[0] = Game::get_instance()->time;
	}
	void TimeCache::new_position(const glm::vec3& newPos) {
		pos[1] = pos[0];
		pos[0] = newPos;
	}
	void TimeCache::calculate_velocities() {
		vel[1] = vel[0];
		vel[0] = (pos[0] - pos[1])/(times[0] - times[1]);
	}
	void TimeCache::calcultate_accelerations() {
		acc[1] = acc[0];
		acc[0] = (vel[0] - vel[1]) / (times[0] - times[1]);
	}
	void TimeCache::calculate_jerk() {
		jerk = (acc[0] - acc[1]) / (times[0] - times[1]);
	}

}