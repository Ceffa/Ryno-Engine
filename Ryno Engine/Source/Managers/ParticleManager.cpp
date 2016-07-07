#include "ParticleManager.h"
namespace Ryno{

	ParticleManager* ParticleManager::get_instance(){

		static ParticleManager instance;//only at the beginning
		return &instance;
	}

	void ParticleManager::update(F32 delta)
	{
		for (const auto go : emitters){
			go->get_script<Emitter>()->update(delta);
		}
	}


	void ParticleManager::reset()
	{
		emitters.clear();
	}

}