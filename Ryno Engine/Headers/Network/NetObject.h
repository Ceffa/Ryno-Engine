#pragma once
#include "Script.h"
#include "NetStructs.h"
#include <set>

namespace Ryno{

	struct TimeCache {
		F32 times[2];
		glm::vec3 pos[2];
		glm::vec3 vel[2];
		glm::vec3 acc[2];
		glm::vec3 jerk;
		glm::vec3 last_predicted_pos;
		void new_position(const glm::vec3& new_pos);
		void calculate_times();
		void calculate_velocities();
		void calcultate_accelerations();
		void calculate_jerk();
		void recalculate(const glm::vec3& last_pos);
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
		F32 last_modified;		//needed to desync the object if inactive

		void set_network_position(const glm::vec3& newPos);

	private:
		F32 last_update = 0;	//needed to send messages in regular time intervals
		bool mark_for_destruction = false;
		static F32 disconnect_delay;
		static F32 send_delay;
		static F32 extrapolation_length;
		TimeCache time_cache;
		bool owned;
	};

}