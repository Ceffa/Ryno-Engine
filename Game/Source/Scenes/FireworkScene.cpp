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

		camera->movement_speed = 2000;
		camera->position = glm::vec4(0, 150, -5000, 1);
		camera->background = ColorRGB(0,17,40);
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

		go[0].transform.set_scale(100, 100, 100);
		go[0].transform.set_position(0, 55, 50);

		auto* dl = go[0].add_component<DirectionalLight>();

		dl->model.material.set_shader(&light_shader);
		dl->set_rotation(-65, 150,0);
		dl->diffuse_intensity = 1.7;
		dl->set_diffuse_color(255, 255, 200);
		dl->specular_intensity = .0;
		dl->set_specular_color(255, 255, 200);
		dl->ambient_intensity = 0;
		dl->set_ambient_color(255, 255, 200);

		shader.create("Geometry/geometry",GAME);
		shader2.create("Geometry/geometry2",GAME);

		auto* emitter = go[0].add_component<Emitter>();
		emitter->save_map.add("texture", &white);
		emitter->save_map.add("normal", &white_normal);
		emitter->save_map.add("mesh", &star_mesh);
		emitter->save_map.add("shad", &shader);

		




		emitter->lambda_creation = [](Emitter* e, GameObject* p){
			Texture* white, *normal;
			Shader* shader;
			//Emitter* emitter;
			I32* mesh;
			white = e->save_map.get<Texture>("texture");
			normal = e->save_map.get<Texture>("normal");
			mesh = e->save_map.get<I32>("mesh");
			shader = e->save_map.get<Shader>("shad");

			auto cmp = p->get_component<Particle3D>();

			cmp->decay_rate = .5f;
			cmp->speed =500;
			auto& sm = p->add_component<Model>()->add_sub_model();
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
				GameObject* p = e->new_particle();
				auto cmp = p->get_component<Particle3D>();
				p->transform.set_position(e->game_object->transform.get_position());
				cmp->direction = ryno_math::get_rand_dir(0, 360, 0, 360);
			}
		};

		emitter->lambda_particle_update = [](Emitter* e, GameObject* p, float _delta_time)
		{
			auto cmp = p->get_component<Particle3D>();
			p->transform.add_position(cmp->direction * cmp->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(30), glm::vec3(50), cmp->lifetime));

			ColorRGBA from(255,255,0,255);
			ColorRGBA to(255,0,0,255);

			Model* model = p->get_component<Model>();
			model->sub_models[0].material.set_attribute("in_DiffuseColor",ryno_math::lerp(from, to, power_lerper(cmp->lifetime,20)));
			model->sub_models[0].material.set_attribute("in_SpecularColor", ColorRGBA(0, 0, 0, 0));

		};

		go[1]=go[0];
		go[1].delete_component<DirectionalLight>();

		go[2]=go[1];
		go[3]=go[1];

		auto* emit_3 = go[3].get_component<Emitter>();
		auto* emit_2 = go[2].get_component<Emitter>();
		auto* emit_1 = go[1].get_component<Emitter>();

				
		emit_3->save_map.add("texture", &brick);
		emit_3->save_map.add("normal", &brick_normal);
		emit_3->save_map.add("mesh", &sphere_mesh);
		emit_2->save_map.add("texture", &white);
		emit_2->save_map.add("normal", &white_normal);
		emit_2->save_map.add("mesh", &cube_mesh);
		emit_2->save_map.add("shad", &shader2);
		emit_1->save_map.add("mesh", &cone_mesh);
		emit_1->save_map.add("texture", &white);
		emit_1->save_map.add("normal", &white_normal);




		emit_1->lambda_particle_update = [](Emitter* e, GameObject* p, float _delta_time)
		{
			auto cmp = p->get_component<Particle3D>();
			p->transform.add_position(cmp->direction * cmp->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(30), glm::vec3(50), cmp->lifetime));

			ColorRGBA from = ColorRGBA::green;
			ColorRGBA to = ColorRGBA::blue;

			Model* model = p->get_component<Model>();
			model->sub_models[0].material.set_attribute("in_DiffuseColor", ryno_math::lerp(from, to, power_lerper(cmp->lifetime, 20)));
			model->sub_models[0].material.set_attribute("in_SpecularColor", ColorRGBA(0, 0, 0, 0));

		};
		emit_2->lambda_particle_update = [](Emitter* e, GameObject* p, float _delta_time)
		{
			auto cmp = p->get_component<Particle3D>();

			p->transform.add_position(cmp->direction * cmp->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(30), glm::vec3(50), cmp->lifetime));

			Model* model = p->get_component<Model>();
			model->sub_models[0].material.set_attribute("in_DiffuseColor", ColorRGBA(255,255,255,0));
			model->sub_models[0].material.set_attribute("in_Color2", ColorRGBA(255,0,0,0));


		};

		emit_3->lambda_particle_update = [](Emitter* e, GameObject* p, float _delta_time)
		{
			auto cmp = p->get_component<Particle3D>();

			p->transform.add_position(cmp->direction * cmp->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(60), glm::vec3(100), cmp->lifetime));
			Model* model = p->get_component<Model>();
			model->sub_models[0].material.set_attribute("in_SpecularColor", ColorRGBA(0, 0, 0, 0));

			
		};
		
		emit_3->lambda_spawn = [](Emitter* e){
			for (U8 i = 0; i < 15; i++) {
				GameObject* p = e->new_particle();
				auto cmp = p->get_component<Particle3D>();

				p->transform.set_position(e->game_object->transform.get_position());
				cmp->direction = ryno_math::get_rand_dir(0, 360, 0, 360);

				Model* model = p->get_component<Model>();
				model->sub_models[0].material.set_attribute("in_DiffuseColor", ryno_math::rand_color_range(ColorRGBA(0, 0, 0, 0), ColorRGBA(255, 255, 255, 0)));
			}
		};
		go[0].get_component<Emitter>()->init(2700);
		emit_1->init(2700);
		emit_2->init(2700);
		emit_3->init(2200);


		go[0].transform.add_position(glm::vec3(1200, 1200, 0));
		go[1].transform.add_position(glm::vec3(-1200, 1200, 0));
		go[2].transform.add_position(glm::vec3(1200, -1200, 0));
		go[3].transform.add_position(glm::vec3(-1200, -1200, 0));

	}


	void FireworkScene::update()
	{
		shader2.set_uniform("g_Time", TimeManager::time);
	}

	void FireworkScene::input(){
		if (!game->shell->active){

			if (game->input_manager->is_key_pressed(SDLK_n, KEYBOARD)){
				idx = ++idx % 4;
			}
			if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)){
				go[idx].transform.add_position(TimeManager::delta_time * 1.0f * glm::vec3(1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)){
				go[idx].transform.add_position(TimeManager::delta_time * 1.0f * glm::vec3(-1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)){
				go[idx].transform.add_position(TimeManager::delta_time * 1.0f * glm::vec3(0,1, 0));
			}
			if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)){
				go[idx].transform.add_position(TimeManager::delta_time * 1.0f * glm::vec3(0, -1, 0));
			}
		}
	}
}