#pragma once

#include "Script.h"

namespace Ryno {

	class ColorLights : public Script {

	public:

		void start() override {
			color_lights();
		}
		void input() override {
			if (Game::get_instance()->input_manager->is_key_pressed(SDLK_v, KEYBOARD))
				color_lights();
		}

		void update() override {}
	

		void color_lights() {
			for (auto* c : gameObject->transform.children)
				c->game_object->spot_light->diffuse_color = ryno_math::rand_color_range(ColorRGBA::black, ColorRGBA::white);
		}


	};

}
