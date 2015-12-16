#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Scene.h"
#include "Game.h"

namespace Ryno{

	class ValleyScene : public Scene
	{

	public:
		ValleyScene() : Scene(){}
	private:
		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;

		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		
		New<GameObject> go;

	
	};
}
