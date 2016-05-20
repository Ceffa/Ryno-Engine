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
		
		camera->position = glm::vec4(0, 50, 0, 1);
		camera->movement_speed /= 10;
		camera->rotation_speed /= 2;
		

		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);

		camera->skybox = game->texture_manager->load_cube_map("full_moon", GAME);

		point_light_shader.create("LightPass/point", ENGINE);

		light.point_light = new PointLight();
		auto* p = light.point_light;
		p->model = new SubModel();
		p->model->material.set_shader(&point_light_shader);
		p->set_diffuse_color(255, 150, 120);
		p->diffuse_intensity = 3;
		p->attenuation = .0005;
		p->specular_intensity =25;
		p->set_specular_color(255, 150,120);

		//Directional light
		dir_light_shader.create("LightPass/directional", ENGINE);

	
		material.create("Geometry/material", GAME);

		sponza.model = game->mesh_manager->load_model("sponza", Owner::GAME, material);
		sponza.transform.set_position(0, 0, 0);
		sponza.transform.set_rotation(0, 90, 0);

		sponza.transform.set_scale(.2f,.2f,.2f);
		sponza.dir_light = new DirectionalLight();
		auto* l = sponza.dir_light;
		l->model = new SubModel();
		l->model->material.set_shader(&dir_light_shader);
		l->set_rotation(-70, 10, 0);
		l->diffuse_intensity = 1.5f;
		l->set_diffuse_color(255, 240, 210);
		l->specular_intensity = 30.0;
		l->set_specular_color(255, 255, 255);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 200);
		


	}


	static bool attach = true;
	void HouseScene::update(){

		if (attach)
			light.transform.set_position(camera->position.x, camera->position.y, camera->position.z);

	}

	void HouseScene::input(){
	
		if (game->input_manager->is_key_pressed(SDLK_SPACE, KEYBOARD)) {
			attach = !attach;
		}
		if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
			sponza.transform.add_rotation(0, 1.5f, 0);
		}
	
		
	}


}