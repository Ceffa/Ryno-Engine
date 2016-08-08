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


		RigidBody* rb = floor.add_script<RigidBody>();
		CollisionHalfPlane* hp = rb->add_primitive<CollisionHalfPlane>();
		hp->normal = glm::vec3(0, 1, 0);
		hp->offset = -50;

		auto& m = ball[0].add_script<Model>()->add_sub_model();


		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = sphere_mesh;
		m.cast_shadows = false;

	

		ball[0].transform.set_scale(11,9,10);
		rb = ball[0].add_script<RigidBody>();
		rb->set_mass(1);
		rb->acceleration = glm::vec3(0, -200, 0);
		ball[0].transform.set_position(0, 40, 0);
		CollisionSphere* c_s = rb->add_primitive<CollisionSphere>();
		c_s->radius = 10;
		rb->set_inertia_tensor(InertiaTensorGenerator::get_tensor(*c_s, rb->get_mass()));


		/*ball[1].copy(ball[0]);
		ball[1].transform.set_position(40, 40, 0);

		ball[2].copy(ball[0]);
		ball[2].transform.set_position(0,0, 0);
		ball[2].get_script<RigidBody>()->acceleration = glm::vec3(0, 0, 0);
		ball[2].get_script<RigidBody>()->set_inverted_mass(1);


		
		ball[3].copy(ball[2]);
		ball[3].transform.set_position(40, 0, 0);*/
		


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


		/*
		spring_force = new Spring(glm::vec3(0,1,0),glm::vec3(0,0,0), b[0], 20, .1f, true);
		reg.add(b[1], spring_force);
		reg.add(b[2], new Spring(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), b[1], 20, .1f, true));*/
		//gravity_force = new Gravity(glm::vec3(0, -10, 0));
		//game->physics_world->add_force(b[0], gravity_force);
		


	
	}

	void PhysicsScene::update() {
		//std::cout << ball[0].get_script<RigidBody>()->rotation.z << std::endl;

	}
	static int curr = 0;
	void PhysicsScene::input() {
	
		float speed = 5;
		glm::vec3 dir = glm::vec3(0,0,0);

		RigidBody* rb = ball[curr].get_script<RigidBody>();
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
			rb->add_orientation(glm::quat(.05f * glm::vec3(sin(game->time/3.0f), sin(game->time), sin (game->time/2.0f))));
		}

		if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)) {
			curr = (curr + 1) % NUM_BODIES;
		}

		rb->velocity += dir;
	

	}
}