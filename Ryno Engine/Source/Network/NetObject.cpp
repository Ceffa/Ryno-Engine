#include "NetObject.h"
#include "NetUtil.h"
#include "Network.h"
#include "Game.h"

namespace Ryno {
	std::set<NetObject*> NetObject::net_objects;
	Ryno::F32 NetObject::disconnect_delay = 2;
	Ryno::F32 NetObject::send_delay = .2f;

	NetObject::NetObject(const NetId& addr) : id(addr), last_sent(0) {
		net_objects.insert(this);
		Client* c = Network::get_instance()->client;
		owned = id.client_id == c->client_id;
	}

	NetObject* NetObject::find(const NetId& id) {
		for (NetObject* obj : net_objects) {
			if (obj->id.equals(id))
				return obj;
		}
		return nullptr;
	}

	void NetObject::set_network_transform(const glm::vec3& new_pos,const glm::quat& new_rot, const glm::vec3& new_scale, F32 last_net_time) {
		time_cache.new_position(new_pos);//probably add lerp here between estimated and actual pos
		time_cache.new_rotation(new_rot);//probably add lerp here between estimated and actual pos
		time_cache.new_scale(new_scale);//probably add lerp here between estimated and actual pos

		time_cache.recalculate(game_object->transform.get_position(),game_object->transform.get_rotation(),game_object->transform.get_scale(),last_net_time);
	}

	void NetObject::reset_network_transform(const glm::vec3& new_pos, const glm::quat& new_rot, const glm::vec3& new_scale, F32 last_net_time) {
		time_cache.pos[0] = new_pos;
		time_cache.pos[1] = new_pos;
		time_cache.rot[0] = new_rot;
		time_cache.rot[1] = new_rot;
		time_cache.scale[0] = new_scale;
		time_cache.scale[1] = new_scale;
		time_cache.d_pos = glm::vec3(0, 0, 0);
		time_cache.d_rot = glm::quat(glm::vec3(0, 0, 0));
		time_cache.d_scale = glm::vec3(0, 0, 0);
		time_cache.last_predicted_pos = new_pos;
		time_cache.last_predicted_rot = new_rot;
		time_cache.last_predicted_scale = new_scale;
		time_cache.times[0] = last_net_time;
		time_cache.times[1] = last_net_time;
		game_object->transform.set_position(new_pos);
		game_object->transform.set_rotation(new_rot);
		game_object->transform.set_scale(new_scale);
	}


	void TimeCache::recalculate(const glm::vec3& last_pos, const glm::quat& last_rot, const glm::vec3& last_scale, F32 last_net_time) {
		last_predicted_pos = last_pos;
		last_predicted_rot = last_rot;
		last_predicted_scale = last_scale;
		calculate_times(last_net_time);
		calculate_velocities();
	}

	void NetObject::update()
	{
		if (!owned) {
			F32 delta_t_net = Network::client->net_time.get_time() - time_cache.times[0];
			F32 delta_t_loc = TimeManager::time - time_cache.last_received_time;
			F32 lerp_net = glm::clamp(delta_t_net / send_delay, 0.0f, 1.0f);
			
			glm::vec3 start_pos = ryno_math::lerp(time_cache.last_predicted_pos, time_cache.pos[0], lerp_net);
			glm::quat start_rot = glm::slerp(time_cache.last_predicted_rot, time_cache.rot[0], lerp_net);
			glm::vec3 start_scale = ryno_math::lerp(time_cache.last_predicted_scale, time_cache.scale[0], lerp_net);

			start_pos += delta_t_net * time_cache.d_pos;
			start_rot = glm::slerp(start_rot, start_rot * time_cache.d_rot,delta_t_net);
			start_scale += delta_t_net * time_cache.d_scale;
			
			game_object->transform.set_position(start_pos);
			game_object->transform.set_rotation(start_rot);
			game_object->transform.set_scale(start_scale);
		}
	}
	void TimeCache::calculate_times(F32 last_net_time) {
		times[1] = times[0];
		times[0] = last_net_time;
		last_received_time = TimeManager::time;
	}
	void TimeCache::new_position(const glm::vec3& newPos) {
		pos[1] = pos[0];
		pos[0] = newPos;
	}
	void TimeCache::new_rotation(const glm::quat& new_rot) {
		rot[1] = rot[0];
		rot[0] = new_rot;
	}
	void TimeCache::new_scale(const glm::vec3& new_scale) {
		scale[1] = scale[0];
		scale[0] = new_scale;
	}
	void TimeCache::calculate_velocities() {
		//Get delta time and error check for nan
		F32 time_delta = times[0] - times[1];
		if (time_delta < .00001f)
			time_delta = .00001f;
		d_pos = (pos[0] - pos[1]) / time_delta;
		d_rot = glm::slerp(glm::quat(glm::vec3(0,0,0)),(glm::inverse(rot[0]) * rot[1]),1.0f/time_delta);
		d_scale = (scale[0] - scale[1])/time_delta;
	
	}
	
}