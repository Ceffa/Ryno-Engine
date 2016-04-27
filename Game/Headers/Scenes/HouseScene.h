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
		Font font;

		GameObject cones[3];
		GameObject walls[5];
		GameObject spot_light_go;
		GameObject directional_light_go;
		GameObject spheres[4];
		GameObject emitter_obj;

		GUIObject gui;


		I32 marker_mesh;
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		Texture bt, bn, solaire, sun, doge, white, white_normal;
		I32 sphere_mesh, cone_mesh, cube_mesh, terrain_mesh;
		Shader shader,dir_light_shader, point_light_shader, spot_light_shader;


	};
}