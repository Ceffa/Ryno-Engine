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
namespace Ryno{
	class SecondScene : public Scene
	{

	public:
		SecondScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;
		
		GameObject* spheres[5];
		GameObject* ball;
		std::list<GameObject*> markers;

		Texture white;
		I32 marker_mesh;
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		GameObject* marker;

		GameObject* particle_batch;
	
		GameObject* body;
		New<GameObject> go;
		New<DirectionalLight> l;
		void BuildQuadForGrid(glm::vec3 position, glm::vec2 uv,bool buildTriangles, int vertsPerRow);
		void NewTerrain();
	
	};
}
