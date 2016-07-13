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

		p[0] = ball[0].add_script<Particle>();
		
		p[0]->set_mass(10);
		p[0]->set_velocity(glm::vec3(0, 0, 0));

		ball[1].copy(ball[0]);
		ball[1].delete_script<DirectionalLight>();
		ball[1].get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", ColorRGBA(255, 100, 0, 255));
		ball[2].copy(ball[1]);
		ball[2].get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", ColorRGBA(0, 255, 255, 255));
		
		p[1] = ball[1].get_script<Particle>();
		p[2] = ball[2].get_script<Particle>();

		ball[1].transform.set_position(1, 1, 0);
		ball[2].transform.set_position(2, 2, 0);


		gravity_force = new ParticleGravity(glm::vec3(0, -15,0));
		//spring_force = new ParticleSpring(ball2.get_script<Particle>(), 50, 1, true);
		//buoyancy_force = new ParticleBuoyancy(15, 6, 1);
		
		p[0]->set_inverted_mass(0);
		reg.add(p[1], gravity_force);
		reg.add(p[2], gravity_force);

		
		cables[0] = new ParticleCable(p[0],p[1],10,.5f);
		cables[1] = new ParticleCable(p[1], p[2],10, .5f);	

		resolver = new ParticleResolver(10);

	}

	void PhysicsScene::update() {
		reg.update_forces(game->delta_time);
		for (Particle* _p : p) _p->integrate(game->delta_time);

		U total = 0;
		for (ParticleCable* _c : cables) {
			if (!_c)continue;
			U detected;
			ParticleContact* boing = _c->add_contact(1, &detected);
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
	
		float speed = 50;
		if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
			ball[1].transform.add_position(game->delta_time * speed * glm::vec3(1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			ball[1].transform.add_position(game->delta_time * speed * glm::vec3(-1, 0, 0));
		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			ball[1].transform.add_position(game->delta_time * speed * glm::vec3(0, 1, 0));
		}
		if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			ball[1].transform.add_position(game->delta_time * speed * glm::vec3(0, -1, 0));
		}

	}
}