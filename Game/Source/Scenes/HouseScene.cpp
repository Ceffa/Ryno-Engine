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
		camera->have_skybox = false;
		camera->background = ColorRGB(0, 0, 0);

		

		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);


		point_light_shader.create("LightPass/point", ENGINE);

		light[0].point_light = new PointLight();
		light[0].transform.set_position(0, 100, 200);
		auto* p = light[0].point_light;
		p->model = new SubModel();
		p->model->material.set_shader(&point_light_shader);
		p->set_diffuse_color(255, 150, 120);
		p->diffuse_intensity = 3;
		p->attenuation = .0005;
		p->specular_intensity =25;
		p->set_specular_color(255, 150,120);

	/*	light[1].copy(light[0]);
		light[1].point_light->set_diffuse_color(255, 100, 100);
		light[1].transform.set_position(0, 100, -200);*/


		

		//Directional light
		dir_light_shader.create("LightPass/directional", ENGINE);

	
		material.create("Geometry/material", GAME);

		//sponza - sibenik
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
		l->set_diffuse_color(255, 235, 200);
		l->specular_intensity = 30.0;
		l->set_specular_color(255, 235, 200);
		l->ambient_intensity = .05f;
		l->set_ambient_color(255, 235, 200);
		material.set_uniform("g_Time", 0);
		material.set_uniform("g_Power", 0);


	}


	static bool attach = false;
	static int power = 0;

	void HouseScene::update(){

		if (attach)
			light[light_index].transform.set_position(camera->position.x, camera->position.y, camera->position.z);
		material.set_uniform("g_Time", game->time);
		if (game->input_manager->is_key_pressed(SDLK_x, KEYBOARD)) {
			power = power == 0 ? 100 : 0;
			material.set_uniform("g_Power", power);
			
			sponza.dir_light->shadows = !sponza.dir_light->shadows;
			for (auto& l : light)
				l.point_light->shadows = !light[0].point_light->shadows;
		}

	
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			light[light_index].point_light->diffuse_intensity -= .05f;
		}else if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
				light[light_index].point_light->diffuse_intensity += .05f;
		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			light[light_index].point_light->specular_intensity += 2.5f;
		}
		else if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			light[light_index].point_light->specular_intensity -= 2.5f;

		}
		if (game->input_manager->is_key_pressed(SDLK_z, KEYBOARD)) {
			ColorRGBA c = ryno_math::rand_color_range(ColorRGBA(100, 100, 100, 255), ColorRGBA::white);
			light[light_index].point_light->diffuse_color = c;
			light[light_index].point_light->specular_color = c;

		}


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