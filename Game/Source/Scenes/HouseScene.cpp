#pragma once
#include "HouseScene.h"
#include "Text.h"
#include "GPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>
#include "Terrain.h"
#include "ValleyScene.h"

namespace Ryno{
	
	void HouseScene::start(){
		

		////initializations
		camera->position = glm::vec4(0, 50, 0, 1);
		camera->movement_speed /= 10;
		camera->rotate(0, 90);
		//////shader
		shader.create("Geometry/geometry",GAME);
		//////loading graphics

		

		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);

		//loading models
		sphere_mesh = game->mesh_manager->load_mesh("sphere", GAME);
		cube_mesh = game->mesh_manager->load_mesh("cube", GAME);
		


		////loading skyboxes
		camera->skybox = game->texture_manager->load_cube_map("full_moon", GAME);

	

		//Base
		walls[0].model = new Model();
		auto& sm = walls[0].model->add_sub_model();
		sm.material.set_shader(&shader);
		sm.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		sm.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));
		sm.material.set_uniform("texture_sampler", white.id);
		sm.material.set_uniform("normal_map_sampler", white_normal.id);

		walls[0].transform.set_scale(3,3,3);
		sm.mesh = sphere_mesh;
		sm.material.set_attribute("in_Tiling", glm::vec2(3,3));
		sm.cast_shadows = false;
		walls[0].transform.set_position(0, 150, 150);

		point_light_shader.create("LightPass/point", ENGINE);

		walls[0].point_light = new PointLight();
		auto* p = walls[0].point_light;
		p->model = new SubModel();
		p->model->material.set_shader(&point_light_shader);
		p->set_diffuse_color(255, 150, 120);
		p->diffuse_intensity = 2;
		p->attenuation = .0001;
		p->specular_intensity =0;
		p->set_specular_color(255, 0, 0);


		////Right
		walls[2].copy(walls[0]);
		walls[2].transform.set_position(0, 60, 210);
		p = walls[2].point_light;
		p->attenuation = 0.002f;
		p->set_diffuse_color(255, 120, 70);
		p->diffuse_intensity = 7;

		//Front
		walls[3].copy(walls[2]);
		walls[3].transform.set_position(0, 70, 0);
		

	

		//////Directional light
		dir_light_shader.create("LightPass/directional", ENGINE);

	
		
		material.create("Geometry/material", GAME);
		//std::cout << "i" << std::endl;
		city.model = game->mesh_manager->load_model("sponza", Owner::GAME, material);
		city.transform.set_position(0, 0, 0);
		city.transform.set_rotation(0, 90, 0);

		city.transform.set_scale(.2f,.2f,.2f);
		city.dir_light = new DirectionalLight();
		auto* l = city.dir_light;
		l->model = new SubModel();
		l->model->material.set_shader(&dir_light_shader);
		l->set_rotation(-70, 10, 0);
		l->diffuse_intensity = .3f;
		l->set_diffuse_color(255, 235, 170);
		l->specular_intensity = .0;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 200);
		l->shadows = false;


	}


	void HouseScene::update(){

		//city.dir_light->rotation = glm::quat(glm::vec3(0, 0.005f, 0)) * city.dir_light->rotation;

	}

	void HouseScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene("firework");
				return;
			}
		}
	
		float speed = 1;
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			walls[0].transform.add_position(0, 0, -speed);
			walls[2].transform.add_position(0, 0, -speed);
			walls[3].transform.add_position(0, 0, -speed);

		}
		if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
			walls[0].transform.add_position(0, 0, speed);
			walls[2].transform.add_position(0, 0, speed);
			walls[3].transform.add_position(0, 0, speed);

		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			walls[0].transform.add_position(0, speed, 0);
			walls[2].transform.add_position(0, speed, 0);
			walls[3].transform.add_position(0, speed, 0);
		}
		if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			walls[0].transform.add_position(0, -speed, 0);
			walls[2].transform.add_position(0, -speed, 0);
			walls[3].transform.add_position(0, -speed, 0);

		}

	}


}