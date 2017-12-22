#pragma once
#include "ValleyScene.h"
#include "FireworkScene.h"
#include "Terrain.h"
#define DEG_TO_RAD 0.0174532925199433

namespace Ryno {

	void ValleyScene::start() {

		game = Game::get_instance();

		camera->position = glm::vec4(-70.85f,185.26f,-312.74f, 1);
		camera->yaw = 0.1995f;
		camera->pitch = 0.6505f;
		camera->movement_speed = 100;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("full_moon", ".png", GAME);

		auto& eff = post_processor.add_effect("PostProcessing/mask", GAME);
		eff.set_uniform("mask", 2);
		post_processor.add_effect("PostProcessing/neg", GAME);

		bt = game->texture_manager->load_png("brick.png", GAME);
		bn = game->texture_manager->load_png("brick_normal.png", GAME);
		cube_mesh = game->mesh_manager->load_mesh("cube", GAME);
		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		shader.create("Geometry/geometry", GAME);

		
		speed = 100;


		SubModel& m = center.add_component<Model>()->add_sub_model();

		m.material.set_shader(&shader);
		m.mesh = cube_mesh;
		m.cast_shadows = false;
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		center.transform.set_scale(5, 5, 5);
		center.transform.set_position(0, 35, 0);


		auto* p = center.add_component<SpotLight>();
		p->set_diffuse_color(255, 80, 0);
		p->diffuse_intensity = 0;
		p->attenuation = 2;
		p->shininess = 8;
		p->specular_intensity = 10000;
		p->set_specular_color(255, 80, 0);
		p->set_rotation(-90, 0, 0);
		p->inner_angle = 40;
		p->outer_angle = 50;
		p->blur = 1;
		p->absolute_movement = false;
		p->shadows = false;
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
		d->set_diffuse_color(255, 255, 255);
		d->specular_intensity = 0;
		d->diffuse_intensity = .3f;
		d->set_rotation(-50, 0, 0);
		d->absolute_movement = false;
		d->blur = 1;
		d->shadows = true;
		d->shadow_strength = 1;
		auto* pl = center.add_component<PointLight>();
		pl->set_diffuse_color(255, 80, 0);
		pl->diffuse_intensity = 0;
		pl->attenuation = 2;
		pl->specular_intensity = 5000;
		pl->shininess = 10;
		pl->set_specular_color(255, 80, 0);
		pl->shadows = true;
		pl->shadow_strength = 1;

		color_lights();


		auto& sm = base.add_component<Model>()->add_sub_model();

		sm.material.set_shader(&shader);
		sm.mesh = cube_mesh;
		sm.cast_shadows = false;
		sm.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 0));
		sm.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		sm.material.set_attribute("in_Tiling", glm::vec2(2, 2));
		sm.material.set_uniform("texture_sampler", bt.id);
		sm.material.set_uniform("normal_map_sampler", bn.id);

		base.transform.set_scale(200, 1, 200);
		base.transform.set_position(0, 0, 0);

		I32 s = 18;//size
		poles.resize(s*s);
		cube_mesh = game->mesh_manager->load_mesh("cube", GAME);

		SubModel& mm = poles[0].add_component<Model>()->add_sub_model();
		mm.material.set_shader(&shader);
		mm.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 0));
		mm.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		mm.material.set_attribute("in_Tiling", glm::vec2(.5f, 5));
		mm.material.set_uniform("texture_sampler", bt.id);
		mm.material.set_uniform("normal_map_sampler", bn.id);
		mm.mesh = cube_mesh;
		mm.cast_shadows = true;

		poles[0].transform.set_scale(2, 20, 2);
		poles[0].transform.set_position(0, 0, 0);
		for (I32 i = 0; i < s; i++)
		{
			for (I32 j = 0; j < s; j++)
			{
				if (i == j && i == 0)continue;
				poles[i*s + j] = poles[0];
				poles[i*s + j].transform.set_position((i - s / 2) * 20, 0, (j - s / 2) * 20);
			}
		}

	}

	void ValleyScene::update() {
		for (auto* c : center.transform.children)
			c->add_rotation(glm::quat(TimeManager::delta_time * speed* glm::vec3(0, +.02f, 0)));
	}

	void ValleyScene::input() {
		if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
			center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(-1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(0, 1, 0));
		}
		if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			center.transform.add_position(TimeManager::delta_time * speed* glm::vec3(0, -1, 0));
		}
		if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
			center.transform.add_rotation(glm::quat(TimeManager::delta_time * speed* glm::vec3(0, +.02f, 0)));
		}
		if (game->input_manager->is_key_down(SDLK_m, KEYBOARD)) {
			center.transform.add_rotation(glm::quat(TimeManager::delta_time * speed* glm::vec3(0, -.02f, 0)));
		}
		if (game->input_manager->is_key_pressed(SDLK_z, KEYBOARD)) {
			for (auto c : center.transform.children) {
				c->game_object->get_component<SpotLight>()->absolute_movement = !c->game_object->get_component<SpotLight>()->absolute_movement;
			}
			center.get_component<DirectionalLight>()->absolute_movement = !center.get_component<DirectionalLight>()->absolute_movement;
		}
		if (Game::get_instance()->input_manager->is_key_pressed(SDLK_v, KEYBOARD))
			color_lights();

	}
}