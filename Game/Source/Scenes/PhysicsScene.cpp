#pragma once

#include "PhysicsScene.h"
#include "GameObject.h"
#include "Particle.h"

namespace RynoEngine {


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

		ball.transform.set_scale(2,2,2);
		auto& m = ball.add_script<Model>()->add_sub_model();


		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = mesh;
		m.cast_shadows = false;

		//dir light
		auto* l = ball.add_script<DirectionalLight>();
		l->model.material.set_shader(&dir_light_shader);
		l->set_rotation(-50, 10, 0);
		l->diffuse_intensity = 1.5f;
		l->set_diffuse_color(255, 235, 200);
		l->specular_intensity = 20;
		l->set_specular_color(0, 0, 0);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 255);
		l->shadows = false;

		Particle* p = ball.add_script<Particle>();
		
		p->set_mass(10);
		p->set_velocity(glm::vec3(0, 0, 0));

		ball2.copy(ball);
		ball2.delete_script<DirectionalLight>();
		ball2.get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", ColorRGBA(255, 100, 0, 255));
		
		gravity_force = new ParticleGravity(glm::vec3(0, -30,0));
		spring_force = new ParticleSpring(ball2.get_script<Particle>(), 50, 1, true);
		reg.add(p, gravity_force);
		reg.add(p, spring_force);
		

	}

	void PhysicsScene::update() {
		reg.update_forces(game->delta_time);
		ball.get_script<Particle>()->integrate(game->delta_time);
	}
	void PhysicsScene::input() {
	
		float speed = 50;
		if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
			ball2.transform.add_position(game->delta_time * speed * glm::vec3(1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			ball2.transform.add_position(game->delta_time * speed * glm::vec3(-1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			ball2.transform.add_position(game->delta_time * speed * glm::vec3(0, 1, 0));
		}
		if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			ball2.transform.add_position(game->delta_time * speed * glm::vec3(0, -1, 0));
		}

	}
}