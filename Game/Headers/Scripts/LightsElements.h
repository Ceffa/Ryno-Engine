#pragma once

#include "Script.h"

namespace Ryno {

	class LightsElements : public Script {


	public:
		Game* game;
		F32 speed;
		std::vector<GameObject> balls;
		Shader point_light_shader, dir_light_shader;
		Texture white, white_normal;
		I32  cube_mesh;
		Shader shader;

		void start() override {
			game = Game::get_instance();
			shader.create("Geometry/geometry", GAME);
			speed = .1f;

			white = game->texture_manager->load_png("white_pixel", GAME);
			white_normal = game->texture_manager->load_png("normal_pixel", GAME);
			cube_mesh = game->mesh_manager->load_mesh("cube", GAME);

			game_object->model = new Model();
			game_object->model->material.set_shader(&shader);
			game_object->model->mesh = cube_mesh;
			game_object->model->cast_shadows = false;
			game_object->model->material.set_attribute("in_Color", ColorRGBA(255, 255, 255, 255));
			game_object->model->material.set_attribute("in_Tiling", glm::vec2(1,1));
			game_object->model->material.set_uniform("texture_sampler", white.id);
			game_object->model->material.set_uniform("normal_map_sampler", white_normal.id);
			game_object->transform.set_scale(5, 5, 5);
			game_object->transform.set_position(0, 35, 0);


			point_light_shader.create("LightPass/spot", ENGINE);

			game_object->spot_light = new SpotLight();
			auto* p = game_object->spot_light;
			p->model = new Model();
			p->model->material.set_shader(&point_light_shader);
			p->set_diffuse_color(255, 80, 0);
			p->diffuse_intensity = 1.7f;
			p->attenuation = .0001;
			p->specular_intensity = 1.5f;
			p->set_specular_color(255, 80, 0);
			p->set_rotation(-90, 0, 0);
			p->cutoff = 30;
			p->absolute_movement = false;

			dir_light_shader.create("LightPass/directional", ENGINE);

			game_object->dir_light = new DirectionalLight();
			auto* d = game_object->dir_light;
			d->model = new Model();
			d->model->material.set_shader(&dir_light_shader);
			d->set_diffuse_color(255, 255, 255);
			d->diffuse_intensity = 0.3f;
			d->set_rotation(-50, 0, 0);
			d->absolute_movement = false;


			U32 nr = 20;
			balls.resize(nr);
			for (I32 i = 0; i < nr; i++) {
				balls[i].copy(*game_object);
				balls[i].transform.set_position(20 * sin(i * 360 / nr * DEG_TO_RAD), 0, 20 * cos(i * 360 / nr * DEG_TO_RAD));
				balls[i].transform.set_scale(.4f, .4f, .4f);
				delete balls[i].dir_light;
				balls[i].dir_light = nullptr;
				balls[i].spot_light->set_rotation(55, 360 / nr * i, 180);
			}
			for (I32 i = 0; i < nr; i++) {
				balls[i].transform.set_parent(&game_object->transform);
			}
			p->cutoff = 45;

			color_lights();

		}
		void input() override {
			if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
				game_object->transform.add_position(game->delta_time * speed* glm::vec3(1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
				game_object->transform.add_position(game->delta_time * speed* glm::vec3(-1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
				game_object->transform.add_position(game->delta_time * speed* glm::vec3(0, 0, 1));
			}
			if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
				game_object->transform.add_position(game->delta_time * speed* glm::vec3(0, 0, -1));
			}
			if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
				game_object->transform.add_rotation(glm::quat(game->delta_time * speed* glm::vec3(0, +.02f, 0)));
			}
			if (game->input_manager->is_key_down(SDLK_m, KEYBOARD)) {
				game_object->transform.add_rotation(glm::quat(game->delta_time * speed* glm::vec3(0, -.02f, 0)));
			}
			if (game->input_manager->is_key_pressed(SDLK_z, KEYBOARD)) {
				for (auto* c : game_object->transform.children) {
					c->game_object->spot_light->absolute_movement = !c->game_object->spot_light->absolute_movement;
				}
				game_object->dir_light->absolute_movement = !game_object->dir_light->absolute_movement;
			}
			if (Game::get_instance()->input_manager->is_key_pressed(SDLK_v, KEYBOARD))
				color_lights();
			
		}

		void update() {
			for (auto* c : game_object->transform.children)
				c->add_rotation(glm::quat(game->delta_time * speed* glm::vec3(0, +.02f, 0)));
		}

		void set_speed(F32 s) {
			speed = s;
		}

		void color_lights() {
			for (auto* a : game_object->transform.children) {
				ColorRGBA c = ryno_math::rand_color_range(ColorRGBA::black, ColorRGBA::white);
				a->game_object->spot_light->diffuse_color = c;
				a->game_object->spot_light->specular_color = c;
			}
		}


	};

}
