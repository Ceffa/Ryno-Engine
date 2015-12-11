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

	class MainScene : public Scene
	{

	public:
		MainScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;

		Texture white;
		I32 marker_mesh;
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		
		New<GameObject> go;
		New<DirectionalLight> l;


		void BuildQuadForGrid(glm::vec3 position, glm::vec2 uv,bool buildTriangles, int vertsPerRow);
		void NewTerrain();
	
	};
}
