#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Scene.h"
#include "Game.h"
#include "Material.h"
#include "GUIObject.h"

namespace Ryno{

	class HouseScene : public Scene
	{

	public:
		HouseScene() : Scene(){}
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
		New<GameObject> directional_light_go;
		New<GameObject> spheres[4];
		New<GameObject> emitter_obj;

		New<GUIObject> gui;


		I32 marker_mesh;
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		New<Texture> bt, bn, solaire, sun, doge, white, white_normal;
		New<I32> sphere_mesh, cone_mesh, cube_mesh, terrain_mesh;
		New<Shader> shader,dir_light_shader, point_light_shader;


	};
}