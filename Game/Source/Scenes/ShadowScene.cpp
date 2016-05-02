#pragma once
#include "ShadowScene.h"
#include "FireworkScene.h"
#include "Terrain.h"

namespace Ryno{

	void ShadowScene::start(){

		camera->position = glm::vec4(0,200,0, 1);
		camera->movement_speed = .01f;

		bt = game->texture_manager->load_png("nipple", GAME);
		bn = game->texture_manager->load_png("nipple_normal", GAME);

		terrain_mesh = game->mesh_manager->create_empty_mesh(GAME);

		game->mesh_builder->set_mesh(terrain_mesh);
		NewTerrain(game->mesh_builder, 100, 2,2, 200);

		camera->skybox = game->texture_manager->load_cube_map("day", GAME);
	
		shader.create("Geometry/geometry",GAME);
		
		go.model = new Model();
		go.model->material.set_shader(&shader);
		go.model->material.set_attribute("in_Color", ColorRGBA(255, 255, 255, 0));
		go.model->material.set_attribute("in_Tiling", glm::vec2(10, 10));
		go.model->material.set_uniform("texture_sampler", bt.id);
		go.model->material.set_uniform("normal_map_sampler", bn.id);
		go.model->mesh = terrain_mesh;
		go.model->cast_shadows = false;

		dir_shader.create("LightPass/directional", ENGINE);

		go.transform = new Transform();
		go.transform->set_scale(1,1,1);
		go.transform->set_position(0, 55, 50);
		go.dir_light = new DirectionalLight();
		auto* l = go.dir_light;
		l->model = new Model();
		l->model->material.set_shader(&dir_shader);
		l->set_direction(-45, 150);
		l->diffuse_intensity = 0.7f;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = 0.1f;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = 0.05f;
		l->set_ambient_color(255, 255, 200);

	}
		

	void ShadowScene::update(){
	}
	
	void ShadowScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene("firework");
				return;
			
			}
		}
	}



}