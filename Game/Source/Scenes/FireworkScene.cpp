#pragma once
#include "FireworkScene.h"
#include "ValleyScene.h"
#include "HouseScene.h"
#include "Batches/Batch3DGeometry.h"
#include "Shader.h"

namespace Ryno{

	F32 power_lerper(F32 i, F32 factor){
		return (std::pow(factor,i) - 1) / (factor -1);
	}

	void FireworkScene::start(){

		camera->movement_speed = 2;
		camera->position = glm::vec4(0, 150, -5000, 1);

		white = game->texture_manager->load_png("white_pixel", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel", GAME);
		brick = game->texture_manager->load_png("pack/154", GAME);
		brick_normal = game->texture_manager->load_png("pack/154_norm", GAME);

		star_mesh = game->mesh_manager->load_mesh("star", 1, GAME);
		cube_mesh= game->mesh_manager->load_mesh("cube", 1, GAME);
		sphere_mesh= game->mesh_manager->load_mesh("sphere", 1, GAME);
		cone_mesh= game->mesh_manager->load_mesh("cone", 1, GAME);


		light_shader.create("LightPass/directional", ENGINE);

		////loading audio
		//sound = game->audio_manager->load_sound("stomp.wav", GAME);
		//music = game->audio_manager->load_music("journey.ogg", GAME);
		//sound.set_volume(1.0f);
		//music.set_volume(1.0f);
		//music.play();
		go[0].transform.set_scale(100, 100, 100);
		go[0].transform.set_position(0, 55, 50);
	/*	go[0].model.create(game->stack_allocator);
		go[0].model->mesh = cube_mesh;
		go[0].model->set_texture_normal(white, white_normal);*/
		go[0].dir_light = new DirectionalLight();
		go[0].dir_light->model = new Model();
		go[0].dir_light->model->material.set_shader(&light_shader);
		go[0].dir_light->set_rotation(-65, 150,0);
		go[0].dir_light->diffuse_intensity = 0.7;
		go[0].dir_light->set_diffuse_color(255, 255, 200);
		go[0].dir_light->specular_intensity = .05;
		go[0].dir_light->set_specular_color(255, 255, 200);
		go[0].dir_light->ambient_intensity = .05;
		go[0].dir_light->set_ambient_color(255, 255, 200);

		shader.create("Geometry/geometry",GAME);
		shader2.create("Geometry/geometry2",GAME);

		go[0].emitter = new Emitter(&go[0]);
		auto* emitter = go[0].emitter;
		emitter->save_map.add("texture", &white);
		emitter->save_map.add("normal", &white_normal);
		emitter->save_map.add("mesh", &star_mesh);
		emitter->save_map.add("shad", &shader);

		




		emitter->lambda_creation = [](Emitter* e, Particle3D* p){
			Texture* white, *normal;
			Shader* shader;
			//Emitter* emitter;
			I32* mesh;
			white = e->save_map.get<Texture>("texture");
			normal = e->save_map.get<Texture>("normal");
			mesh = e->save_map.get<I32>("mesh");
			shader = e->save_map.get<Shader>("shad");


			p->decay_rate = .0005f;
			p->speed = .5f;
			p->model = new Model();
			p->model->material.set_shader(shader);
			p->model->mesh = *mesh;
			p->model->material.set_attribute("in_Color",ColorRGBA::yellow);
			p->model->material.set_attribute("in_Tiling", glm::vec2(1,1));
			p->model->material.set_uniform("texture_sampler", white->id);
			p->model->material.set_uniform("normal_map_sampler", normal->id);

			//p->model->color = ColorRGBA::yellow;
			p->transform.set_rotation(ryno_math::rand_int_range(0, 360), ryno_math::rand_int_range(0, 360), 0);
			p->model->cast_shadows = false;
			
		};
		emitter->lambda_spawn = [](Emitter* e){
			for (U8 i = 0; i < 20; i++){
				Particle3D* p = e->new_particle();
				p->transform.set_position(e->game_object->transform.get_position());
				p->direction = ryno_math::get_rand_dir(0, 360, 0, 360);
			}
		};

		emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{
			p->transform.add_position(p->direction * p->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(30), glm::vec3(50), p->lifetime));

			ColorRGBA from = ColorRGBA::yellow;
			ColorRGBA to = ColorRGBA::red;

			p->model->material.set_attribute("in_Color",ryno_math::lerp(from, to, power_lerper(p->lifetime,20)));
		};

		go[1].copy(go[0]);
		delete go[1].dir_light;
		go[1].dir_light = nullptr;

		go[2].copy(go[1]);
		go[3].copy(go[1]);

		go[3].emitter->save_map.add("texture", &brick);
		go[3].emitter->save_map.add("normal", &brick_normal);
		go[3].emitter->save_map.add("mesh", &sphere_mesh);
		go[2].emitter->save_map.add("texture", &white);
		go[2].emitter->save_map.add("normal", &white_normal);
		go[2].emitter->save_map.add("mesh", &cube_mesh);
		go[2].emitter->save_map.add("shad", &shader2);
		go[1].emitter->save_map.add("mesh", &cone_mesh);
		go[1].emitter->save_map.add("texture", &white);
		go[1].emitter->save_map.add("normal", &white_normal);




		go[1].emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{
			p->transform.add_position(p->direction * p->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(30), glm::vec3(50), p->lifetime));

			ColorRGBA from = ColorRGBA::green;
			ColorRGBA to = ColorRGBA::blue;

			p->model->material.set_attribute("in_Color", ryno_math::lerp(from, to, power_lerper(p->lifetime, 20)));
		};
		go[2].emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{
			p->transform.add_position(p->direction * p->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(30), glm::vec3(50), p->lifetime));
			p->model->material.set_attribute("in_Color", ColorRGBA::white);
			p->model->material.set_attribute("in_Color2", ColorRGBA::red);


		};

		go[3].emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{
			p->transform.add_position(p->direction * p->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(60), glm::vec3(100), p->lifetime));

			
		};
		
		go[3].emitter->lambda_spawn = [](Emitter* e){
			for (U8 i = 0; i < 15; i++){
				Particle3D* p = e->new_particle();
				p->transform.set_position(e->game_object->transform.get_position());
				p->direction = ryno_math::get_rand_dir(0, 360, 0, 360);
				p->model->material.set_attribute("in_Color", ryno_math::rand_color_range(ColorRGBA(100, 100, 100, 0), ColorRGBA(255,255,255,0)));
			}
		};
		go[0].emitter->init(2700);
		go[1].emitter->init(2700);
		go[2].emitter->init(2700);
		go[3].emitter->init(2200);


		go[0].transform.add_position(glm::vec3(1200, 1200, 0));
		go[1].transform.add_position(glm::vec3(-1200, 1200, 0));
		go[2].transform.add_position(glm::vec3(1200, -1200, 0));
		go[3].transform.add_position(glm::vec3(-1200, -1200, 0));

	}


	void FireworkScene::update()
	{
		shader2.set_uniform("g_Time", game->time);
	}

	void FireworkScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene("house");
				return;

			}
			if (game->input_manager->is_key_pressed(SDLK_n, KEYBOARD)){
				idx = ++idx % 4;
			}
			if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)){
				go[idx].transform.add_position(game->delta_time * 1.0f * glm::vec3(1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)){
				go[idx].transform.add_position(game->delta_time * 1.0f * glm::vec3(-1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)){
				go[idx].transform.add_position(game->delta_time * 1.0f * glm::vec3(0,1, 0));
			}
			if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)){
				go[idx].transform.add_position(game->delta_time * 1.0f * glm::vec3(0, -1, 0));
			}
		}
	}



}