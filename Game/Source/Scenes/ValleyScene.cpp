#pragma once
#include "ValleyScene.h"
#include "FireworkScene.h"
#include "Terrain.h"

namespace Ryno{

	void ValleyScene::start(){

		camera->position = glm::vec4(0,200,0, 1);
		camera->movement_speed = .01f;

		Texture bt = game->texture_manager->load_png("nipple", GAME);
		Texture bn = game->texture_manager->load_png("nipple_normal", GAME);

		I32 terrain_mesh = game->mesh_manager->create_empty_mesh(GAME);

		game->mesh_builder->set_mesh(terrain_mesh);
		NewTerrain(game->mesh_builder, 100, 2,2, 200);

		camera->skybox = game->texture_manager->load_cube_map("day", GAME);
		
		////loading audio
		//sound = game->audio_manager->load_sound("stomp.wav", GAME);
		//music = game->audio_manager->load_music("journey.ogg", GAME);
		//sound.set_volume(1.0f);
		//music.set_volume(1.0f);
		//music.play();

		go.create(game->stack_allocator);
		go->model.create(game->stack_allocator);
		go->model->set_color_and_flatness(255, 255, 255, 0);
		go->model->set_tiling(10, 10);
		go->model->set_texture_normal(bt, bn);
		go->model->mesh = terrain_mesh;
		go->transform.create(game->stack_allocator);
		go->transform->set_scale(1,1,1);
		go->transform->set_position(0, 55, 50);
		go->dir_light.create(game->stack_allocator);
		go->dir_light->set_direction(-45, 150);
		go->dir_light->diffuse_intensity = 0.7f;
		go->dir_light->set_diffuse_color(255, 255, 200);
		go->dir_light->specular_intensity = 0.1f;
		go->dir_light->set_specular_color(255, 255, 200);
		go->dir_light->ambient_intensity = 0.05f;
		go->dir_light->set_ambient_color(255, 255, 200);

	}
		

	void ValleyScene::update(){
	}
	
	void ValleyScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene("firework");

			}
		}
	}



}