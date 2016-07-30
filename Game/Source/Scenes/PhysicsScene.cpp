#pragma once

#include "PhysicsScene.h"
#include "GameObject.h"
#include "Particle.h"

#define CUBE_SIZE;
namespace Ryno {

	void PhysicsScene::start() {

		camera->position = glm::vec4(0,0,-100, 1);
		camera->yaw = 0;
		camera->pitch = 0;
		camera->movement_speed = 100;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("full_moon", ".png", GAME);


		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		mesh = game->mesh_manager->load_mesh("sphere", GAME);

		shader.create("Geometry/geometry", GAME);
		dir_light_shader.create("LightPass/directional", ENGINE);

		ball[0].transform.set_scale(2,2,2);

		auto& m = ball[0].add_script<Model>()->add_sub_model();


		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = mesh;
		m.cast_shadows = false;

	

		b[0] = ball[0].add_script<RigidBody>();
		
		b[0]->set_mass(10);
		b[0]->velocity = glm::vec3(0, 0, 0);


		F32 restit = .5f;
		F32 length = 15;
		for (int i = 0; i < NUM_BODIES; i++) {
			if (i != 0)
				ball[i].copy(ball[0]);
			ball[i].transform.set_position((i-1) * 40, 0, 0);
			b[i] = ball[i].get_script<RigidBody>();
			b[i]->acceleration = glm::vec3(0,-20,0);		
		}

		b[0]->acceleration = glm::vec3(0, 0, 0);
		b[0]->set_inverted_mass(0);

		//dir light
		auto* l = ball[0].add_script<DirectionalLight>();
		l->model.material.set_shader(&dir_light_shader);
		l->set_rotation(-50, 10, 0);
		l->diffuse_intensity = 1.5f;
		l->set_diffuse_color(255, 235, 200);
		l->specular_intensity = 20;
		l->set_specular_color(0, 0, 0);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 255);
		l->shadows = false;


		
		spring_force = new Spring(glm::vec3(0,0,0),glm::vec3(0,0,0), b[0], 20, .1f, true);
		reg.add(b[1], spring_force);
		reg.add(b[2], new Spring(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), b[1], 20, .1f, true));
	

	}

	void PhysicsScene::update() {
		reg.update_forces(game->delta_time);
		for (RigidBody* _b : b) _b->integrate(game->delta_time);

	}
	void PhysicsScene::input() {
	
		float speed = 1.5f;
		glm::vec3 dir = glm::vec3(0,0,0);
		if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
			dir += glm::vec3(speed, 0, 0);
		}
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			dir += glm::vec3(-speed, 0, 0);
		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			dir +=  glm::vec3(0, speed, 0);
		}
		if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			dir += glm::vec3(0, -speed, 0);
		}

		b[0]->add_position(dir);
	

	}
}