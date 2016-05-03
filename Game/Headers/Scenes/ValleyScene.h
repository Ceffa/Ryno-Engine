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
		Shader shader, dir_shader, point_light_shader;
		Texture bt, bn, white,white_normal;
		I32 terrain_mesh, cube_mesh,sphere_mesh;
		GameObject go,sphere;
		std::vector<GameObject> poles;
		std::vector<GameObject> balls;
	};
}
