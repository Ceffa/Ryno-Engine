#include "MainScene.h"
#include "Text.h"
#include "GPUProfiler.h"
#include "CPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>
#include "SphereCollider.h"
#include "PointCollider.h"
#include "AABBCollider.h"
#include "ConvexCollider.h"
#include "GJK.h"
#include "PerlinNoise.h"
#include "SecondScene.h"

namespace Ryno{

	void MainScene::start(){

		//initializations
		game->camera->position = glm::vec4(0,150,-500, 1);


		//loading graphics
	
		Texture texture_bricks = game->texture_manager->load_png("pack/177", GAME);
		Texture normal_map_bricks = game->texture_manager->load_png("pack/177_norm", GAME);
		Texture at = game->texture_manager->load_png("pack/196", GAME);
		Texture an = game->texture_manager->load_png("pack/196_norm", GAME);
		Texture bt = game->texture_manager->load_png("pack/161", GAME);
		Texture bn = game->texture_manager->load_png("pack/161_norm", GAME);
		Texture solaire = game->texture_manager->load_png("solaire", GAME);
		Texture sun = game->texture_manager->load_png("sun", GAME);
		Texture doge = game->texture_manager->load_png("doge", GAME);
		white = game->texture_manager->load_png("white_pixel", GAME);
		Texture white_normal = game->texture_manager->load_png("normal_pixel", GAME);


		//loading models
		static I32 sphere_mesh = game->mesh_manager->load_mesh("sphere", 1, GAME);
		static I32 cone_mesh = game->mesh_manager->load_mesh("cone", 1, GAME);
		static I32 cube_mesh = game->mesh_manager->load_mesh("cube", 1, GAME);
		
		static I32 terrain_mesh = game->mesh_manager->create_empty_mesh();
		game->mesh_builder->set_mesh(terrain_mesh);
		

		//loading skyboxes
		game->camera->skybox = game->texture_manager->load_cube_map("full_moon_small", GAME);


		//loading audio
		sound = game->audio_manager->load_sound("stomp.wav", GAME);
		music = game->audio_manager->load_music("journey.ogg", GAME);
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();
	

		//Build the environnement

		//Cones
		go = New<GameObject>(new GameObject());
		go->model = New<Model> (new Model());
		go->transform = New<Transform>(new Transform());
		go->model->set_color_and_flatness(255, 255, 255, 0);
		go->model->set_texture_normal(white, white_normal);
		go->model->mesh = cone_mesh;
		go->transform->set_scale(100,100,100);
		go->transform->set_position(0, 55, 50);
		
		
		go->dir_light = New<DirectionalLight>(new DirectionalLight());
		go->dir_light->set_direction(-65, 150);
		go->dir_light->diffuse_intensity = 0.2;
		go->dir_light->set_diffuse_color(255, 255, 200);
		go->dir_light->specular_intensity = .05;
		go->dir_light->set_specular_color(255, 255, 200);
		go->dir_light->ambient_intensity = .05;
		go->dir_light->set_ambient_color(255, 255, 200);
	
	}
		

	void MainScene::update(){
		/*go->transform->set_scale(99, 99, 99);
		go->dir_light->set_direction(-65, 151);*/


	}
	
	void MainScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene(new SecondScene());

			}
		}
	}



}