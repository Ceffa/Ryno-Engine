#pragma once
#include "ValleyScene.h"
#include "FireworkScene.h"
#include "Terrain.h"

namespace Ryno{

	void ValleyScene::start(){

		camera->position = glm::vec4(0,200,0, 1);
		camera->movement_speed = .01f;

		bt.create(game->stack_allocator, game->texture_manager->load_png("nipple", GAME));
		bn.create(game->stack_allocator, game->texture_manager->load_png("nipple_normal", GAME));

		terrain_mesh.create(game->stack_allocator, game->mesh_manager->create_empty_mesh(GAME));

		game->mesh_builder->set_mesh(**terrain_mesh);
		NewTerrain(game->mesh_builder, 100, 2,2, 200);

		camera->skybox = game->texture_manager->load_cube_map("day", GAME);
		
		shader.create(game->stack_allocator);
		shader->create("GeometryPass/geometry", 1, 0, 1);
		

		mod.create(game->stack_allocator);
		mod->material.create(game->stack_allocator);
		mod->material->set_shader(*shader);
		mod->material->set_attribute("in_Color", ColorRGBA(255, 255, 255, 0));
		mod->material->set_attribute("in_Tiling", glm::vec2(10, 10));
		mod->material->set_uniform("texture_sampler", bt->id);
		mod->material->set_uniform("normal_map_sampler", bn->id);
		mod->mesh = **terrain_mesh;
		mod->cast_shadows = false;

		go.create(game->stack_allocator);

		go->model.copy(mod);
		
		
		
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