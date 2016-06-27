#pragma once
#include "MinecraftScene.h"
#include "Text.h"
#include "GPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>
#include "Terrain.h"
#include "ValleyScene.h"

namespace Ryno{
	
	void MinecraftScene::start(){
		
		camera->position = glm::vec4(-20.79f, 53.84f, -185, 1);
		camera->yaw = 0.1155f;
		camera->pitch = 0.1625f;
		camera->movement_speed /= 20;
		camera->rotation_speed /= 2;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("day", GAME);

		

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
		sponza.model = game->mesh_manager->load_model("lost_empire", Owner::GAME, material);
		sponza.transform.set_position(0, 0, 0);
		sponza.transform.set_rotation(0, 0, 0);

		sponza.transform.set_scale(2,2,2);
		sponza.dir_light = new DirectionalLight();
		auto* l = sponza.dir_light;
		l->model = new SubModel();
		l->model->material.set_shader(&dir_light_shader);
		l->set_rotation(-70, 10, 0);
		l->diffuse_intensity = 3;
		l->set_diffuse_color(255, 255, 255);
		l->specular_intensity = 0;
		l->set_specular_color(255, 255, 255);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 255);
		material.set_uniform("g_Time", 0);
		material.set_uniform("g_Power", 0);

		for (auto& m : sponza.model->sub_models) {
			m.cast_shadows = false;
			ColorRGBA* attr = (ColorRGBA*)m.material.get_attribute("diffuse_color");
			if (attr != nullptr) {
				ColorRGBA c = *attr;
				c.a = 100;
				m.material.set_attribute("diffuse_color", c);
			}
			
		}


	}


	static bool attach = false;
	static int power = 0;

	void MinecraftScene::update(){

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
	void MinecraftScene::input(){
	
		if (game->input_manager->is_key_pressed(SDLK_SPACE, KEYBOARD)) {
			attach = !attach;
		}
		if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
			sponza.transform.add_rotation(0, 1.5f, 0);
		}

		
	
		
	}


}