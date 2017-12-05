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
		ValleyScene() : Scene() {}
		virtual ~ValleyScene() {}
	private:
		void start() override;
		void input() override;
		void update() override;
		
		std::vector<GameObject> poles;
		Texture bt, bn;
		Game* game;
		GameObject base;
		F32 speed;
		std::vector<GameObject> balls;
		Texture white, white_normal;
		I32  cube_mesh;
		Shader shader;
		GameObject center;

		void color_lights() {
			for (auto* a : center.transform.children) {
				ColorRGBA c = ryno_math::rand_color_range(ColorRGBA::black, ColorRGBA::white);
				a->game_object->get_component<SpotLight>()->specular_color = c;

			}
		}
	};
}
