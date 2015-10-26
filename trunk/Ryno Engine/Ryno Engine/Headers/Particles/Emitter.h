#ifndef emit_def
#define emit_def
#include "Global.h"
#include "Model.h"
#include "Structures.h"
#include "GameObject.h"
#include "Lerp.h"
#include <vector>
#include <functional>

namespace Ryno{
	

	class Particle3D : public GameObject {
	public:
		Particle3D(){}
		Particle3D(GameObject* go);
		~Particle3D(){} 
		glm::vec3 direction;
		F32 speed;
		ColorRGBA color;
		F32 lifetime;
		

	};

class Emitter{

	friend class Particle3D;
	
	public:
		Emitter(){}
		Emitter(const Emitter *e);
		~Emitter();
		void init(U32 nr_particles, F32 decay_rate, F32 numbers_per_frame, GameObject* go, std::function<void(Particle3D*, F32)> update_func);
		void add_particle(const glm::vec3& position, const glm::vec3& dir, F32 speed, const glm::vec3& size, const ColorRGBA color);
		Particle3D* find_free_particle();
		void update(F32 delta_time);

		GameObject* game_object;

		std::function<void(Particle3D*, F32)> lambda_particle;
		std::function<void(F32)> lambda_emitter;
		F32 m_max_particles;
		F32 m_decay_rate;
		F32 m_number_per_frame;
	private:
		

		

		std::vector <Particle3D*> m_particles;
	};


}
#endif 