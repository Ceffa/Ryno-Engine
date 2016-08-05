#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include "RigidBody.h"
#include "ForceGenerator.h"
#include "ForceRegister.h"
#include "Emitter.h"
#include "Global.h"
#include "Scene.h"
#include "Game.h"
#include "Narrow.h""

#include <vector>

#define NUM_BODIES 4

namespace Ryno{
	class PhysicsScene : public Scene
	{

	public:
		PhysicsScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void update() override;
	
		I32 sphere_mesh, cube_mesh;
		Shader shader, dir_light_shader;
		Texture white, white_normal;
	
		GameObject ball[NUM_BODIES];
		RigidBody* b[NUM_BODIES];
		GameObject s;

		ForceRegister reg;
		Gravity* gravity_force;
		Spring* spring_force;
		

		



	};
}
