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
		
		camera->position = glm::vec4(-6.5f,30,-261, 1);
		camera->movement_speed = 100;
		camera->rotation_speed /= 2;
		camera->have_skybox = false;

		

		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);

		camera->background = ColorRGB(0,0,0);

		point_light_shader.create("LightPass/point", ENGINE);

		light[0].transform.set_position(0, 100, 200);
		auto* p = light[0].add_component<PointLight>();
		p->model.material.set_shader(&point_light_shader);
		p->set_diffuse_color(255, 150, 120);
		p->diffuse_intensity = 3;
		p->attenuation = .0005;
		p->specular_intensity =25;
		p->set_specular_color(255, 150,120);

	

		//Directional light
		dir_light_shader.create("LightPass/directional", ENGINE);

	
		material.create("Geometry/material", GAME);

		//sponza - sibenik
		sponza.add_component(game->mesh_manager->load_model("sponza", Owner::GAME, material));
		sponza.transform.set_position(0, 0, 0);
		sponza.transform.set_rotation(0, 90, 0);

		sponza.transform.set_scale(.2f,.2f,.2f);
	
		auto* l = sponza.add_component<DirectionalLight>();
		l->model.material.set_shader(&dir_light_shader);
		l->set_rotation(-70, 10, 0);
		l->diffuse_intensity = 2;
		l->set_diffuse_color(255, 235, 200);
		l->specular_intensity = 30.0;
		l->set_specular_color(255, 235, 200);
		l->ambient_intensity = .25f;
		l->set_ambient_color(255, 235, 200);
		l->blur = 2;
		l->shadow_strength = .8f;

		material.set_uniform("g_Time", 0);
		material.set_uniform("g_Power", 0);


	}


	bool attach = true;
	int power = 0;

	void HouseScene::update(){

	

		if (attach)
			light[light_index].transform.set_position(camera->position.x, camera->position.y, camera->position.z);
		material.set_uniform("g_Time", TimeManager::time * 1000);
		if (game->input_manager->is_key_pressed(SDLK_x, KEYBOARD)) {
			power = power == 0 ? 100 : 0;
			material.set_uniform("g_Power", power);
			
			sponza.get_component<DirectionalLight>()->shadows = !sponza.get_component<DirectionalLight>()->shadows;
			for (auto& l : light)
				l.get_component<PointLight>()->shadows = !light[0].get_component<PointLight>()->shadows;
		}

	
		if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
			light[light_index].get_component<PointLight>()->diffuse_intensity -= .05f;
		}else if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
				light[light_index].get_component<PointLight>()->diffuse_intensity += .05f;
		}
		if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
			light[light_index].get_component<PointLight>()->specular_intensity += 2.5f;
		}
		else if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
			light[light_index].get_component<PointLight>()->specular_intensity -= 2.5f;

		}
		if (game->input_manager->is_key_pressed(SDLK_z, KEYBOARD)) {
			ColorRGBA c = ryno_math::rand_color_range(ColorRGBA(100, 100, 100, 255), ColorRGBA::white);
			light[light_index].get_component<PointLight>()->diffuse_color = c;
			light[light_index].get_component<PointLight>()->specular_color = c;

		}


	}
	void HouseScene::input() {

		if (game->input_manager->is_key_pressed(SDLK_SPACE, KEYBOARD)) {
			attach = !attach;
		}
	}
}