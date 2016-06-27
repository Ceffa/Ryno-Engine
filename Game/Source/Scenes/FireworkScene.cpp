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
		camera->background = ColorRGB(0,50,90);
		camera->have_skybox = false;


		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		brick = game->texture_manager->load_png("pack/154.png", GAME);
		brick_normal = game->texture_manager->load_png("pack/154_norm.png", GAME);

		star_mesh = game->mesh_manager->load_mesh("star", GAME);
		cube_mesh= game->mesh_manager->load_mesh("cube", GAME);
		sphere_mesh= game->mesh_manager->load_mesh("sphere", GAME);
		cone_mesh= game->mesh_manager->load_mesh("cone", GAME);


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
		go[0].dir_light->model = new SubModel();
		go[0].dir_light->model->material.set_shader(&light_shader);
		go[0].dir_light->set_rotation(-65, 150,0);
		go[0].dir_light->diffuse_intensity = 1.7;
		go[0].dir_light->set_diffuse_color(255, 255, 200);
		go[0].dir_light->specular_intensity = .0;
		go[0].dir_light->set_specular_color(255, 255, 200);
		go[0].dir_light->ambient_intensity = 0;
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
			auto& sm = p->model->add_sub_model();
			sm.material.set_shader(shader);
			sm.mesh = *mesh;
			sm.material.set_attribute("in_DiffuseColor",ColorRGBA(255,0,0,0));


			sm.material.set_attribute("in_Tiling", glm::vec2(1,1));
			sm.material.set_uniform("texture_sampler", white->id);
			sm.material.set_uniform("normal_map_sampler", normal->id);

			p->transform.set_rotation(ryno_math::rand_int_range(0, 360), ryno_math::rand_int_range(0, 360), 0);
			sm.cast_shadows = false;
			
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

			ColorRGBA from(255,255,0,255);
			ColorRGBA to(255,0,0,255);

			p->model->sub_models[0].material.set_attribute("in_DiffuseColor",ryno_math::lerp(from, to, power_lerper(p->lifetime,20)));
			p->model->sub_models[0].material.set_attribute("in_SpecularColor", ColorRGBA(0, 0, 0, 0));

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

			p->model->sub_models[0].material.set_attribute("in_DiffuseColor", ryno_math::lerp(from, to, power_lerper(p->lifetime, 20)));
			p->model->sub_models[0].material.set_attribute("in_SpecularColor", ColorRGBA(0, 0, 0, 0));

		};
		go[2].emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{
			p->transform.add_position(p->direction * p->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(30), glm::vec3(50), p->lifetime));
			p->model->sub_models[0].material.set_attribute("in_DiffuseColor", ColorRGBA(255,255,255,0));
			p->model->sub_models[0].material.set_attribute("in_Color2", ColorRGBA(255,0,0,0));


		};

		go[3].emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{
			p->transform.add_position(p->direction * p->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(60), glm::vec3(100), p->lifetime));
			p->model->sub_models[0].material.set_attribute("in_SpecularColor", ColorRGBA(0, 0, 0, 0));

			
		};
		
		go[3].emitter->lambda_spawn = [](Emitter* e){
			for (U8 i = 0; i < 15; i++) {
				Particle3D* p = e->new_particle();
				p->transform.set_position(e->game_object->transform.get_position());
				p->direction = ryno_math::get_rand_dir(0, 360, 0, 360);
				p->model->sub_models[0].material.set_attribute("in_DiffuseColor", ryno_math::rand_color_range(ColorRGBA(0, 0, 0, 0), ColorRGBA(255, 255, 255, 0)));
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