#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Global.h"
#include "Scene.h"
#include "Game.h"
#include "ParticleForceRegister.h"
#include "ParticleContactGenerator.h"
#include "ParticleResolver.h"
#include <vector>

#define LATO_I 2
#define LATO_J 8

namespace Ryno{
	class PhysicsScene : public Scene
	{

	public:
		PhysicsScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void update() override;
	
		I32 mesh;
		Shader shader, dir_light_shader;
		Texture white, white_normal;
	
		
		GameObject ball[LATO_I*LATO_J];
		Particle* p[LATO_I*LATO_J];
		GameObject s;

		ParticleForceRegister reg;
		ParticleGravity* gravity_force;
		ParticleSpring* spring_force;
		ParticleBuoyancy* buoyancy_force;
		std::vector<ParticleCable> cables;
		std::vector<ParticleRod> rods;

		ParticleResolver* resolver;
		std::vector<ParticleContact*> contacts;



	};
}
