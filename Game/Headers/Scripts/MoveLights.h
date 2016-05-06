#pragma once

#include "Script.h"

namespace Ryno {

	class MoveLights : public Script {


	public:
		Game* game;
		F32 speed;
		void start() override {
			game = Game::get_instance();
			speed = .05f;
		}
		void input() override {
			if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
				gameObject->transform.add_position(game->delta_time * speed* glm::vec3(1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
				gameObject->transform.add_position(game->delta_time * speed* glm::vec3(-1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
				gameObject->transform.add_position(game->delta_time * speed* glm::vec3(0, 0, 1));
			}
			if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
				gameObject->transform.add_position(game->delta_time * speed* glm::vec3(0, 0, -1));
			}
			if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
				gameObject->transform.add_rotation(glm::quat(game->delta_time * speed* glm::vec3(0, +.02f, 0)));
			}
			if (game->input_manager->is_key_down(SDLK_m, KEYBOARD)) {
				gameObject->transform.add_rotation(glm::quat(game->delta_time * speed* glm::vec3(0, -.02f, 0)));
			}
			if (game->input_manager->is_key_pressed(SDLK_v, KEYBOARD)) {
				for (auto* c : gameObject->transform.children) {
					c->game_object->spot_light->absolute_movement = !c->game_object->spot_light->absolute_movement;
				}
			}
			
		}

		void update() {
			for (auto* c : gameObject->transform.children)
				c->add_rotation(glm::quat(game->delta_time * speed* glm::vec3(0, +.02f, 0)));
		}

		void set_speed(F32 s) {
			speed = s;
		}


	};

}
