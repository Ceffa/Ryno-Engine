#pragma once

#include "PhysicsScene.h"
#include "GameObject.h"
#include "Particle.h"
#include "InertiaTensorGenerator.h"


namespace Ryno {

	void PhysicsScene::start() {

		camera->position = glm::vec4(0,0,-100, 1);
		camera->yaw = 0;
		camera->pitch = 0;
		camera->movement_speed = 100;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("full_moon", ".png", GAME);


		white = game->texture_manager->load_png("pack/161.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		sphere_mesh = game->mesh_manager->load_mesh("sphere", GAME);
		cube_mesh = game->mesh_manager->load_mesh("cube", GAME);


		shader.create("Geometry/geometry", GAME);
		dir_light_shader.create("LightPass/directional", ENGINE);


		auto& m = ball[0].add_script<Model>()->add_sub_model();


		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = sphere_mesh;
		m.cast_shadows = false;

	

		b[0] = ball[0].add_script<RigidBody>();
		ball[0].transform.set_scale(10,10,10);
		b[0]->set_mass(10);
		ball[0].transform.set_position(0, 40, 0);

		ball[1].copy(ball[0]);

		CollisionSphere* c_s = b[0]->add_primitive<CollisionSphere>();
		c_s->radius = 10;
		b[0]->set_inertia_tensor(InertiaTensorGenerator::get_tensor(*c_s,b[0]->get_mass()));
	
		
		ball[1].transform.set_position(0,0, 0);
		b[1] = ball[1].get_script<RigidBody>();
		CollisionBox* c_b = b[1]->add_primitive<CollisionBox>();
		c_b->half_size = glm::vec3(10,10,10);
		b[1]->set_inertia_tensor(InertiaTensorGenerator::get_tensor(*c_b, b[1]->get_mass()));
		ball[1].get_script<Model>()->sub_models[0].mesh = cube_mesh;
		ball[1].transform.set_rotation(0, 45,0);

		ball[2].copy(ball[0]);
		b[2] = ball[2].get_script<RigidBody>();
		ball[2].transform.set_position(40, 40, 0);
		ball[3].copy(ball[1]);
		b[3] = ball[3].get_script<RigidBody>();
		ball[3].transform.set_position(40, 0, 0);




	


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


		
		//spring_force = new Spring(glm::vec3(0,1,0),glm::vec3(0,0,0), b[0], 20, .1f, true);
		//reg.add(b[1], spring_force);
		//reg.add(b[2], new Spring(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), b[1], 20, .1f, true));
	

		collision_data.setup(5);

	}

	void PhysicsScene::update() {
		for (auto rb : b)
			rb->calculate_derived_data();

		reg.update_forces(game->delta_time);
		static CollisionSphere& pr_a = (CollisionSphere&)(*b[0]->primitives.back());
		static CollisionBox& pr_b = (CollisionBox&)(*b[1]->primitives.back());
		static CollisionSphere& pr_c = (CollisionSphere&)(*b[2]->primitives.back());
		static CollisionBox& pr_d = (CollisionBox&)(*b[3]->primitives.back());
		static Material& m_a = ball[0].get_script<Model>()->sub_models[0].material;
		static Material& m_b = ball[1].get_script<Model>()->sub_models[0].material;
		static Material& m_c = ball[2].get_script<Model>()->sub_models[0].material;
		static Material& m_d = ball[3].get_script<Model>()->sub_models[0].material;

		m_a.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m_b.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m_c.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m_d.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));

		if (CollisionDetector::box_and_sphere(pr_b, pr_a , collision_data) > 0) {
			m_a.set_attribute("in_DiffuseColor", ColorRGBA(255,0,0,255));
			m_b.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
		}
		if (CollisionDetector::sphere_and_sphere(pr_c, pr_a, collision_data) > 0) {
			m_a.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
			m_c.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
		}
		if (CollisionDetector::box_and_sphere(pr_d, pr_a, collision_data) > 0) {
			m_a.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
			m_d.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
		}
		if (CollisionDetector::box_and_sphere(pr_b, pr_c, collision_data) > 0) {
			m_b.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
			m_c.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
		}
		if (CollisionDetector::box_and_box(pr_b, pr_d, collision_data) > 0) {
			m_b.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
			m_d.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
		}
		if (CollisionDetector::box_and_sphere(pr_d, pr_c, collision_data) > 0) {
			m_c.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
			m_d.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));
		}
		
		for (RigidBody* _b : b) _b->integrate(game->delta_time);


		collision_data.clear();

	}
	static int curr = 0;
	void PhysicsScene::input() {
	
		float speed = .5f;
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
		if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
			b[curr]->add_orientation(glm::quat(.05f * glm::vec3(sin(game->time/3.0f), sin(game->time), sin (game->time/2.0f))));
		}

		if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)) {
			curr = (curr + 1) % NUM_BODIES;
		}

		b[curr]->add_position(dir);
	

	}
}