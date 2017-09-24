#pragma once

#include "LightsScene.h"

namespace Ryno {

	void LightsScene::start() {

		camera->position = glm::vec4(0, 308.567f, -727.473, 1);
		camera->yaw = 0;
		camera->pitch = 0.407f;
		camera->movement_speed = 100;
		camera->have_skybox = false;
		camera->background = ColorRGB(0, 0, 0);


		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("pack/161_norm.png", GAME);
		
		mesh = game->mesh_manager->load_mesh("cube", GAME);
		sphere = game->mesh_manager->load_mesh("sphere", GAME);

		shader.create("Geometry/geometry", GAME);
		point_light_shader.create("LightPass/point", ENGINE);



		base.transform.set_scale(2500, 5, 2500);
		base.transform.set_position(0, 0, 0);
		SubModel& m = base.add_component<Model>()->add_sub_model();

		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 0));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_Tiling", glm::vec2(100, 100));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = mesh;
		m.cast_shadows = false;
		
		lights.resize(lato * lato);
		lights[0] = base;
	
		auto* p = lights[0].add_component<PointLight>();
		p->model.material.set_shader(&point_light_shader);
		p->diffuse_intensity = 1.75f;
		p->specular_intensity = 0;
		p->set_specular_color(0, 0, 0);
		p->attenuation = .04f;
		p->shadows = false;

		
		lights[0].delete_component<Model>();


		
		parent.transform.set_position(0, 0, 0);
		parent.transform.set_scale(1, 1, 1);
		for (int i = 0; i < lato; i++)
			for (int j = 0; j < lato; j++) {
				if (!(i == j && j == 0))
					lights[i*lato + j]=lights[0];
				
				lights[i*lato + j].transform.set_position(i * 30 - 30 *lato/2 , 6.5f, j * 30 - 30 * lato/2);
				lights[i*lato + j].transform.set_parent(&parent.transform);
				lights[i*lato + j].get_component<PointLight>()->diffuse_color = ryno_math::rand_color_range(ColorRGBA(70,70,70,255),ColorRGBA(255,255,255,255));
			}






	
	}

	void LightsScene::update() {
		
		static F32 speed = 100;

		if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
			parent.transform.add_position(TimeManager::delta_time * speed * glm::vec3(1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			parent.transform.add_position(TimeManager::delta_time * speed * glm::vec3(-1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			parent.transform.add_position(TimeManager::delta_time * speed * glm::vec3(0, 0, 1));
		}
		if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			parent.transform.add_position(TimeManager::delta_time * speed * glm::vec3(0, 0, -1));
		}
		if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
			parent.transform.set_rotation(3*sin(TimeManager::time ) * glm::vec3 (0,0,1));
		}
		if (game->input_manager->is_key_down(SDLK_m, KEYBOARD)) {
			parent.transform.add_rotation(TimeManager::delta_time / 100.0f * glm::vec3(0, 1, 0));
		}
	
	}

	void LightsScene::input() {
	}
}