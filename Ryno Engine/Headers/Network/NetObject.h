#pragma once
#include "Component.h"
#include "NetStructs.h"
#include <set>

namespace Ryno{
	//This struct keeps track about times
	//and old information received through the network.
	//Every NetworkObject holds one, and use it to predict and interpolate
	struct TimeCache {
		F32 lag;		//Latency of the last received packet
		F32 last_recv;	//Local time of the last received packet
		F32 times[2];	//Last two times received, needed to get delta_t for velocity calculation
		//last two pos,rot and scales. Need two to calculate velocities
		glm::vec3 pos[2];			
		glm::quat rot[2];				
		glm::vec3 scale[2];				
		//velocities
		glm::vec3 d_pos;				
		glm::quat d_rot;				
		glm::vec3 d_scale;				
		//position,rotation and scale of the object when the packet is received
		glm::vec3 last_predicted_pos;	
		glm::quat last_predicted_rot;	
		glm::vec3 last_predicted_scale;

		//Register a new pos/rot/scale
		void new_position(const glm::vec3& new_pos);
		void new_rotation(const glm::quat& new_rot);
		void new_scale(const glm::vec3& new_scale);
		//Update times
		void calculate_times(F32 last_net_time);
		//Update velocities
		void calculate_velocities();
		//Call this function to fully update the time cache after receiving a new packet
		void recalculate(const glm::vec3& last_pos, const glm::quat& last_rot, const glm::vec3& last_scale,F32 last_net_time);
	};

	//NetObject is the component (inherits from Script) that makes an object a network entity.
	class NetObject : public Component {
		friend class Client;
		friend class NetworkScene;
	public:
		NetObject(const NetId& addr);
		~NetObject() { net_objects.erase(this);  }
		NetObject(const NetObject& cp) = delete;
		NetObject() = delete;

		//Holds the client id and the local object id.
		//Together they are a univoque key
		NetId id;

		//All the network objects in the scene
		static std::set<NetObject*> net_objects;

		//Check if the network object is already instantiated
		static NetObject* find(const NetId& id);

		//Called every frame
		void update() override;

		//Keep track of the last time the object was updated 
		//trough the network. Needed to remove object if inactive
		F32 last_received;		

		//Add a new pos/rot/scale to the time cache -> interpolation
		void set_network_transform(const glm::vec3& new_pos, const glm::quat& new_rot, const glm::vec3& new_scale, F32 last_net_time);
		//Hard reset of pos/rot/scale -> no interpolation
		void reset_network_transform(const glm::vec3& new_pos, const glm::quat& new_rot, const glm::vec3& new_scale, F32 last_net_time);

		//A field used to mark different kind of objects, like a player and a ball
		U32 tag;
		//Keep track of the last sent packet, needed to send messages at regular intervals
		F32 last_sent = 0;	
		
		//Other methods inherited from Script
		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };
		NetObject* clone() override { return nullptr; }

	private:
		//If true, object is destroyed at the end of frames
		bool mark_for_destruction = false;
		//After how long an inactive object is destroyed
		static F32 disconnect_delay;
		//Frequence of packet updates
		static F32 send_delay;
		//Holds all the information needed for interpolation and prediction
		TimeCache time_cache;
		//If true, this objects sends update, if false it receives them
		bool owned;
	};

}