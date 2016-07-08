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
		camera->skybox = game->texture_manager->load_cube_map("day", ".png", GAME);

		

		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		

		//Directional light
		dir_light_shader.create("LightPass/directional", ENGINE);

	
		material.create("Geometry/material", GAME);

		//sponza - sibenik
		sponza.add_script(game->mesh_manager->load_model("lost_empire", Owner::GAME, material));
		sponza.transform.set_position(0, 0, 0);
		sponza.transform.set_rotation(0, 0, 0);

		sponza.transform.set_scale(2,2,2);
		auto* l = sponza.add_script<DirectionalLight>();
		l->model.material.set_shader(&dir_light_shader);
		l->set_rotation(-70, 10, 0);
		l->diffuse_intensity = 3;
		l->set_diffuse_color(255, 255, 255);
		l->specular_intensity = 0;
		l->set_specular_color(255, 255, 255);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 255);
		l->shadows = false;
		material.set_uniform("g_Time", 0);
		material.set_uniform("g_Power", 0);

		for (auto m : sponza.get_script<Model>()->sub_models) {
			m->cast_shadows = false;
			ColorRGBA* attr = (ColorRGBA*)m->material.get_attribute("diffuse_color");
			if (attr != nullptr) {
				ColorRGBA c = *attr;
				c.a = 100;
				m->material.set_attribute("diffuse_color", c);
			}
			
		}


	}



	void MinecraftScene::update(){

	

	}
	void MinecraftScene::input(){
	
		
		if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
			sponza.transform.add_rotation(0, 1.5f, 0);
		}

		
	
		
	}


}