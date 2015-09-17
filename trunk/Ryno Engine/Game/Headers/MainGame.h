#pragma once
#include "MainGameInterface.h"
#include "Lights/PointLight.h"
#include "DeferredRenderer.h"
#include "Lights/DirectionalLight.h"
#include <Sprite.h>
#include "Forces.h"

namespace Ryno{
	class MainGame : public MainGameInterface
	{
	private:
	
		void start() override;
		void input() override;
		void update() override;
		void set_physics();

		Sound sound;
		Music music;
		
		GameObject* spheres[4];
		GameObject* ball;
		std::vector<GameObject*> markers;

		WeightForce Fw;
		DragForce Fd;
		Texture white;
		I32 marker_mesh;
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		GameObject* marker;

		
		

		
	
	};
}
