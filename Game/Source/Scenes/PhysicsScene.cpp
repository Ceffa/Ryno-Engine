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

		

		p[0] = ball[0].add_script<Particle>();
		
		p[0]->set_mass(10);
		p[0]->set_velocity(glm::vec3(0, 0, 0));


		F32 restit = .5f;
		F32 length = 15;
		for (int i = 0; i < LATO; i++) {
			for (int j = 0; j < LATO; j++) {
				int n = i * LATO + j;
				if (i != 0 || j != 0)
					ball[i * LATO + j].copy(ball[0]);
				ball[n].transform.set_position(i * 10, -j * 10, 0);
				p[n] = ball[n].get_script<Particle>();
				p[n]->set_acceleration(glm::vec3(0,-15,0));

			}
		}
		for (int i = 0; i < LATO; i++) {
			for (int j = 0; j < LATO-1; j++) {
				int n = i * LATO + j;
					cables.emplace_back(p[n], p[i * LATO + j + 1], length, restit);
				/*if (i < LATO - 1)
					cables.emplace_back(p[n], p[(i + 1) * LATO + j], length, restit);*/

			}
		}

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


		
		//spring_force = new ParticleSpring(ball2.get_script<Particle>(), 50, 1, true);
		//buoyancy_force = new ParticleBuoyancy(15, 6, 1);
		for (int i = 0; i < LATO; i++)
			p[i*LATO]->set_inverted_mass(0);
	


		resolver = new ParticleResolver(100);

	}

	void PhysicsScene::update() {
		reg.update_forces(game->delta_time);
		for (Particle* _p : p) _p->integrate(game->delta_time);

		U total = 0;
		for (ParticleCable& _c : cables) {
			U detected;
			ParticleContact* boing = _c.add_contact(1, &detected);
			total += detected;
			if (detected == 1)
				contacts.push_back(boing);
		}
		resolver->resolve_contacts(contacts, game->delta_time);
		
		for (ParticleContact* pc : contacts)
			delete pc;
		contacts.clear();
	}
	void PhysicsScene::input() {
	
		float speed = .2f;
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
		for (int i = 0; i < LATO; i++)
			p[i*LATO]->add_position(dir);

	}
}