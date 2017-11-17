#pragma once

#include "Component.h"

namespace Ryno {

	class LightsElements : public Component {


	public:
		Game* game;
		F32 speed;
		std::vector<GameObject> balls;
		Texture white, white_normal;
		I32  cube_mesh;
		Shader shader;
		GameObject center;
		

		LightsElements* clone() override {
			return new LightsElements(*this);
		}

		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };

		void start() override {

			

			game = Game::get_instance();
			shader.create("Geometry/geometry", GAME);
			speed = 100;

			white = game->texture_manager->load_png("white_pixel.png", GAME);
			white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
			cube_mesh = game->mesh_manager->load_mesh("cube", GAME);

			auto& m = center.add_component<Model>()->add_sub_model();

			m.material.set_shader(&shader);
			m.mesh = cube_mesh;
			m.cast_shadows = false;
			m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
			m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));
			m.material.set_attribute("in_Tiling", glm::vec2(1,1));
			m.material.set_uniform("texture_sampler", white.id);
			m.material.set_uniform("normal_map_sampler", white_normal.id);
			center.transform.set_scale(5, 5, 5);
			center.transform.set_position(0, 35, 0);


			auto* p = center.add_component<SpotLight>();
			p->set_diffuse_color(255, 80, 0);
			p->diffuse_intensity = 3;
			p->attenuation = .0005;
			p->specular_intensity = 1;
			p->set_specular_color(255, 80, 0);
			p->set_rotation(-90, 0, 0);
			p->cutoff =40;
			p->absolute_movement = false;
			p->shadows = true;
			p->shadow_strength = 1;

			


			U32 nr = 20;
			balls.resize(nr);
			for (I32 i = 0; i < nr; i++) {

				balls[i] = center;

				balls[i].transform.set_position(20 * sin(i * 360 / nr * DEG_TO_RAD), 0, 20 * cos(i * 360 / nr * DEG_TO_RAD));
				balls[i].transform.set_scale(.4f, .4f, .4f);
				
				balls[i].get_component<SpotLight>()->set_rotation(55, 360 / nr * i, 180);
			}
			for (I32 i = 0; i < nr; i++) {
				balls[i].transform.set_parent(&center.transform);
			}
			
			center.delete_component<SpotLight>();

			auto* d = center.add_component<DirectionalLight>();
			d->set_diffuse_color(255, 255, 0);
			d->diffuse_intensity = .35f;
			d->set_rotation(-50, 0, 0);
			d->absolute_movement = false;
			d->blur = 1;
			d->shadows = true;
			auto* pl = center.add_component<PointLight>();
			pl->set_diffuse_color(255, 80, 0);
			pl->diffuse_intensity = 1;
			pl->attenuation = .0001;
			pl->specular_intensity = 20;
			pl->set_specular_color(0, 0, 0);
			pl->shadows = true;
			pl->shadow_strength = 1;

			color_lights();

		}
		void input() override {
			if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
				center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
				center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(-1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
				center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(0, 0, 1));
			}
			if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
				center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(0, 0, -1));
			}
			if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
				center.transform.add_rotation(glm::quat(TimeManager::delta_time * speed* glm::vec3(0, +.02f, 0)));
			}
			if (game->input_manager->is_key_down(SDLK_m, KEYBOARD)) {
				center.transform.add_rotation(glm::quat(TimeManager::delta_time * speed* glm::vec3(0, -.02f, 0)));
			}
			if (game->input_manager->is_key_pressed(SDLK_z, KEYBOARD)) {
				for (auto* c : center.transform.children) {
					center.get_component<SpotLight>()->absolute_movement = !center.get_component<SpotLight>()->absolute_movement;
				}
				center.get_component<DirectionalLight>()->absolute_movement = !center.get_component<DirectionalLight>()->absolute_movement;
			}
			if (Game::get_instance()->input_manager->is_key_pressed(SDLK_v, KEYBOARD))
				color_lights();
			
		}

		void update() {
			for (auto* c : center.transform.children)
				c->add_rotation(glm::quat(TimeManager::delta_time * speed* glm::vec3(0, +.02f, 0)));
		}

		void set_speed(F32 s) {
			speed = s;
		}

		void color_lights() {
			for (auto* a : center.transform.children) {
				ColorRGBA c = ryno_math::rand_color_range(ColorRGBA::black, ColorRGBA::white);
				a->game_object->get_component<SpotLight>()->diffuse_color = c;
			
			}
		}


	};

}
