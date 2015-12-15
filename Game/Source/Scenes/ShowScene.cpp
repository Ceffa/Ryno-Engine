#pragma once
#include "ShowScene.h"
#include "Text.h"
#include "GPUProfiler.h"
#include "CPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>
#include "GJK.h"
#include "Terrain.h"
#include "MainScene.h"

namespace Ryno{
	
	void ShowScene::start(){
		

		//initializations
		camera->position = glm::vec4(0, 150, -500, 1);


		//loading graphics
		CPUProfiler::begin();
		CPUProfiler::start_time();
		Texture bt = game->texture_manager->load_png("pack/161", GAME);
		Texture bn = game->texture_manager->load_png("pack/161_norm", GAME);
		Texture solaire = game->texture_manager->load_png("solaire", GAME);
		Texture sun = game->texture_manager->load_png("sun", GAME);
		Texture doge = game->texture_manager->load_png("doge", GAME);
		Texture white = game->texture_manager->load_png("white_pixel", GAME);
		Texture white_normal = game->texture_manager->load_png("normal_pixel", GAME);

		CPUProfiler::next_time();

		//loading models
		I32 sphere_mesh = game->mesh_manager->load_mesh("sphere", 1, GAME);
		I32 cone_mesh = game->mesh_manager->load_mesh("cone", 1, GAME);
		I32 cube_mesh = game->mesh_manager->load_mesh("cube", 1, GAME);
		I32 terrain_mesh = game->mesh_manager->create_empty_mesh(GAME);

		CPUProfiler::next_time();

		game->mesh_builder->set_mesh(terrain_mesh);
		NewTerrain(game->mesh_builder,80,5,5,50);

		CPUProfiler::next_time();

		//loading skyboxes
		camera->skybox = game->texture_manager->load_cube_map("full_moon", GAME);

		CPUProfiler::end_time();
		CPUProfiler::print_time();
		////loading audio
		//sound = game->audio_manager->load_sound("stomp.wav", GAME);
		//music = game->audio_manager->load_music("journey.ogg", GAME);
		//sound.set_volume(1.0f);
		//music.set_volume(1.0f);
		//music.play();

		//Environnement

		//Center big Cone
		cones[0].create(game->stack_allocator);
		cones[0]->model.create(game->stack_allocator);
		cones[0]->model->set_color_and_flatness(255, 255, 255, 0);
		cones[0]->model->set_texture_normal(white, white_normal);
		cones[0]->model->mesh = cone_mesh;
		cones[0]->transform.create(game->stack_allocator);
		cones[0]->transform->set_scale(100, 100, 100);
		cones[0]->transform->set_position(0, 55, 50);

		//Left small cone
		cones[1].create(game->stack_allocator,*cones[0]);
		cones[1]->transform->set_scale(30, 30, 30);
		cones[1]->transform->set_position(-150, 20, -170);

		//Right small cone
		cones[2].create(game->stack_allocator, *cones[1]);
		cones[2]->transform->set_position(150, 20, -170);

		//Base
		walls[0].create(game->stack_allocator, *cones[2]);
		walls[0]->transform->set_scale(1, 1, 1);
		walls[0]->model->set_texture_normal(bt, bn);
		walls[0]->model->mesh = terrain_mesh;
		walls[0]->model->set_tiling(3, 3);
		walls[0]->transform->set_position(5, -15, -5);

		//Left
		walls[1].create(game->stack_allocator, *walls[0]);
		walls[1]->model->mesh = cube_mesh;
		walls[1]->transform->set_scale(5, 100, 200);
		walls[1]->transform->set_position(-200, 105, 0);

		//Right
		walls[2].create(game->stack_allocator, *walls[1]);
		walls[2]->transform->set_scale(5, 100, 200);
		walls[2]->transform->set_position(200, 105, 0);

		//Front
		walls[3].create(game->stack_allocator, *walls[2]);
		walls[3]->transform->set_scale(200, 100, 5);
		walls[3]->transform->set_position(0, 105, 200);

		//Roof
		walls[4].create(game->stack_allocator, *walls[3]);
		walls[4]->transform->set_scale(200, 5, 200);
		walls[4]->transform->set_position(0, 205, 0);

		//Spot light
		spot_light.create(game->stack_allocator);
		spot_light->transform.create(game->stack_allocator);
		spot_light->transform->set_position(0, 190, 50);
		SpotLight* s = spot_light->spot_light.create(game->stack_allocator);

		s->set_direction(-90, 0);
		s->cutoff = 30;
		s->set_diffuse_color(0, 255, 0);
		s->diffuse_intensity = 30;
		s->attenuation = .001;
		s->specular_intensity = 10;
		s->set_specular_color(0, 255, 0);


		
		//Point lights
		//Point light 1
		spheres[0].create(game->stack_allocator,*walls[3]);
		spheres[0]->model->set_color_and_flatness(255, 255, 255, 255);
		spheres[0]->model->cast_shadows = false;
		spheres[0]->model->set_texture_normal(white, white_normal);
		spheres[0]->transform->set_scale(12, 12, 12);
		spheres[0]->transform->set_position(180, 20, 180);
		spheres[0]->model->mesh = sphere_mesh;

		PointLight* p = spheres[0]->point_light.create(game->stack_allocator);
		p->set_diffuse_color(255, 80, 0);
		p->diffuse_intensity = 3;
		p->attenuation = .001;
		p->specular_intensity = 10;
		p->set_specular_color(255, 80, 0);

		//Point light 2
		spheres[1].create(game->stack_allocator, *spheres[0]);
		spheres[1]->transform->set_position(-180, 20, 180);

		//Point light 3
		spheres[2].create(game->stack_allocator, *spheres[1]);
		spheres[2]->transform->set_position(-180, 20, -180);

		//Point light 4
		spheres[3].create(game->stack_allocator, *spheres[2]);
		spheres[3]->transform->set_position(180, 20, -180);

		//Directional light
		directional_light.create(game->stack_allocator);
		DirectionalLight* l = directional_light->dir_light.create(game->stack_allocator);
		directional_light->transform.create(game->stack_allocator);
		l->set_direction(-65, 150);
		l->diffuse_intensity = 0.2;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = .05;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = .05;
		l->set_ambient_color(255, 255, 200);

		//GUI
		font.create(game->stack_allocator,"Aaargh", 24, GAME);
		gui.create(game->stack_allocator);
		
		Sprite* sp = gui->sprite.create(game->stack_allocator);
		Text* t = gui->text.create(game->stack_allocator);
		sp->depth = 20;
		sp->angle = 0;
		sp->set_color(255, 255, 255, 255);
		sp->set_texture(doge);
		sp->set_position(0.25, 0.75);
		sp->anchor_point = CENTER;
		sp->set_scale(150, 150);
		sp->set_tiling(1, 1);
		sp->angle = 0;
		t->depth = 10;
		t->font = *font;
		t->set_position(1, 1);
		t->anchor_point = TOP_RIGHT;
		t->set_scale(1, 1);
		t->set_color(255, 255, 0, 255);
		t->text = "Ryno Engine";

		emitter_obj.create(game->stack_allocator);
		emitter_obj->transform.create(game->stack_allocator);
		emitter_obj->transform->set_position(0, 105, 50);
		Emitter* emitter = emitter_obj->emitter.create(game->stack_allocator, *emitter_obj);
		emitter->save_map.add("texture", white);
		emitter->save_map.add("normal", white_normal);
		emitter->save_map.add("mesh", sphere_mesh);




		emitter->lambda_creation = [](Emitter* e, Particle3D* p){
			Texture white, normal;
			//Emitter* emitter;
			I32 mesh;
			e->save_map.get("texture", &white);
			e->save_map.get("normal", &normal);
			e->save_map.get("mesh", &mesh);
			//e->save_map.get("emitter", &emitter);

			p->decay_rate = .001f;
			p->speed = .05f;
			p->model.create(StackAllocator::get_instance());
			p->model->set_texture_normal(white, normal);
			p->model->mesh = mesh;
			p->model->color = ColorRGBA::yellow;
			//p->set_emitter(new Emitter(emitter));
		};
		emitter->lambda_spawn = [](Emitter* e){
			for (U8 t = 0; t < 2; t++){
				Particle3D* p = e->new_particle();
				p->transform->position = e->game_object->transform->position;
				p->direction = ryno_math::get_rand_dir(0, 360, 0, 360);
				//bool b = false;
				//p->get_emitter()->save_map.replace("go_crazy", b);
			}
		};

		emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{

			//bool f = false;
			//bool t = true;
			//if (p->lifetime < .75f){
			p->transform->set_position(p->direction * p->speed * _delta_time + p->transform->position);
			p->transform->set_scale(ryno_math::lerp(glm::vec3(.1), glm::vec3(5), p->lifetime));

			p->model->set_color(255, ryno_math::lerp(0, 255, p->lifetime), 0);
			//}

			/*if (p->lifetime > .75f && p->lifetime < .82f){
			p->get_emitter()->save_map.replace("go_crazy", t);
			if (p->transform->scale.x > 1)
			p->transform->set_scale(0,0,0);
			}
			else p->get_emitter()->save_map.replace("go_crazy", f);*/

		};

		

		//e2->init(200);
		emitter->init(200);
		


	}


	void ShowScene::update(){

		for (I32 i = 0; i < 4; i++){
			spheres[i]->transform->position.y = 40 + sin(game->time_manager->current_time / 1000.0f) * 20;
		}

	}

	void ShowScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene(new MainScene());

			}
			/*if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)){
				particle_batch->transform->add_position(-.5f* game->delta_time, 0, 0);
			}
			if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)){
				particle_batch->transform->add_position(.5f * game->delta_time, 0, 0);
			}
			if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)){
				particle_batch->transform->add_position(0, .5f* game->delta_time, 0);
			}
			if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)){
				particle_batch->transform->add_position(0, -.5f* game->delta_time, 0);
			}
			if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)){
				particle_batch->transform->add_position(0, 0, .5f* game->delta_time);
			}
			if (game->input_manager->is_key_down(SDLK_m, KEYBOARD)){
				particle_batch->transform->add_position(0, 0, -.5f* game->delta_time);
			}*/
		}
	}


}