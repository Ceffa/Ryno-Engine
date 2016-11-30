#pragma once
#include "Script.h"
#include "NetStructs.h"
#include <set>

namespace Ryno{

	struct TimeCache {
		F32 lag;
		F32 times[2];
		glm::vec3 pos[2];
		glm::vec3 d_pos;
		glm::quat rot[2];
		glm::quat d_rot;
		glm::vec3 scale[2];
		glm::vec3 d_scale;
		glm::vec3 last_predicted_pos;
		glm::quat last_predicted_rot;
		glm::vec3 last_predicted_scale;
		void new_position(const glm::vec3& new_pos);
		void new_rotation(const glm::quat& new_rot);
		void new_scale(const glm::vec3& new_scale);
		void calculate_times(F32 last_net_time);
		void calculate_velocities();
		void recalculate(const glm::vec3& last_pos, const glm::quat& last_rot, const glm::vec3& last_scale,F32 last_net_time);
	};

	class NetObject : public Script {
		friend class Client;
		friend class NetworkScene;
	public:
		NetObject(const NetId& addr);
		~NetObject() { net_objects.erase(this);  }
		NetObject(const NetObject& cp) = delete;
		NetObject() = delete;

		NetId id;
		static std::set<NetObject*> net_objects;
		static NetObject* find(const NetId& id);
		NetObject* clone() override { return nullptr; }
		void update() override;
		void input() override {}
		void start() override {}
		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };
		F32 last_received;		//needed to desync the object if inactive

		//Add a new position to the time cache buffer
		void set_network_transform(const glm::vec3& new_pos, const glm::quat& new_rot, const glm::vec3& new_scale, F32 last_net_time);
		//Hard reset of a position to avoid interpolation
		void reset_network_transform(const glm::vec3& new_pos, const glm::quat& new_rot, const glm::vec3& new_scale, F32 last_net_time);

		U32 tag;//used to differentiate types of objects
		F32 last_sent = 0;	//needed to send messages in regular time intervals
	private:
		bool mark_for_destruction = false;
		static F32 disconnect_delay;
		static F32 send_delay;
		TimeCache time_cache;
		bool owned;
	};

}