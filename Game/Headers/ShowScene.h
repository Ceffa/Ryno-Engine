#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Scene.h"
#include "Game.h"
#include "GUIObject.h"

namespace Ryno{

	class ShowScene : public Scene
	{

	public:
		ShowScene() : Scene(){}
	private:

		void start() override;
		void input() override;
		void update() override;

		Sound sound;
		Music music;
		New<Font> font;

		New<GameObject> cones[3];
		New<GameObject> walls[5];
		New<GameObject> spot_light;
		New<GameObject> directional_light;
		New<GameObject> spheres[4];
		New<GameObject> emitter_obj;

		New<GUIObject> gui;

		GameObject* ball;
		std::list<GameObject*> markers;

		I32 marker_mesh;
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		GameObject* marker;

		GameObject* particle_batch;

		GameObject* body;

		void BuildQuadForGrid(glm::vec3 position, glm::vec2 uv, bool buildTriangles, int vertsPerRow);
		void NewTerrain();

	};
}