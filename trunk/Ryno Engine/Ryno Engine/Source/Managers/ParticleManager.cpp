#include "ParticleManager.h"
namespace Ryno{

	ParticleManager* ParticleManager::get_instance(){

		static ParticleManager instance;//only at the beginning
		return &instance;
	}

	void ParticleManager::update(F32 delta)
	{
		U32 tot = 0;
		for (GameObject* go : emitters){
			++tot;
			go->get_emitter()->update(delta);
		}
	}

}