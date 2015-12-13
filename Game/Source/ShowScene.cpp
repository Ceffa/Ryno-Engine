#include "ShowScene.h"
#include "Text.h"
#include "GPUProfiler.h"
#include "CPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>
#include "MainScene.h"
#include "GJK.h"
#include "PerlinNoise.h"

namespace Ryno{

	void ShowScene::start(){
		CPUProfiler::begin();
		CPUProfiler::start_time();

		//initializations
		camera->position = glm::vec4(0, 150, -500, 1);

		CPUProfiler::next_time();

		//loading graphics

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

		game->mesh_builder->set_mesh(terrain_mesh);
		NewTerrain();

		CPUProfiler::next_time();

		//loading skyboxes
		camera->skybox = game->texture_manager->load_cube_map("full_moon_small", GAME);

		CPUProfiler::next_time();

		//loading audio
		sound = game->audio_manager->load_sound("stomp.wav", GAME);
		music = game->audio_manager->load_music("journey.ogg", GAME);
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();


		CPUProfiler::next_time();

		//Environnement

		//Center big Cone
		cones[0].create(game->mallocator);
		cones[0]->model.create(game->mallocator);
		cones[0]->model->set_color_and_flatness(255, 255, 255, 0);
		cones[0]->model->set_texture_normal(white, white_normal);
		cones[0]->model->mesh = cone_mesh;
		cones[0]->transform.create(game->mallocator);
		cones[0]->transform->set_scale(100, 100, 100);
		cones[0]->transform->set_position(0, 55, 50);

		//Left small cone
		cones[1].create(game->mallocator,*cones[0]);
		cones[1]->transform->set_scale(30, 30, 30);
		cones[1]->transform->set_position(-150, 20, -170);

		//Right small cone
		cones[2].create(game->mallocator, *cones[1]);
		cones[2]->transform->set_position(150, 20, -170);

		//Base
		walls[0].create(game->mallocator, *cones[2]);
		walls[0]->transform->set_scale(1, 1, 1);
		walls[0]->model->set_texture_normal(bt, bn);
		walls[0]->model->mesh = terrain_mesh;
		walls[0]->model->set_tiling(3, 3);
		walls[0]->transform->set_position(5, 5, -5);

		//Left
		walls[1].create(game->mallocator, *walls[0]);
		walls[1]->model->mesh = cube_mesh;
		walls[1]->transform->set_scale(5, 100, 200);
		walls[1]->transform->set_position(-200, 105, 0);

		//Right
		walls[2].create(game->mallocator, *walls[1]);
		walls[2]->transform->set_scale(5, 100, 200);
		walls[2]->transform->set_position(200, 105, 0);

		//Front
		walls[3].create(game->mallocator, *walls[2]);
		walls[3]->transform->set_scale(200, 100, 5);
		walls[3]->transform->set_position(0, 105, 200);

		//Roof
		walls[4].create(game->mallocator, *walls[3]);
		walls[4]->transform->set_scale(200, 5, 200);
		walls[4]->transform->set_position(0, 205, 0);

		//Spot light
		spot_light.create(game->mallocator);
		spot_light->transform.create(game->mallocator);
		spot_light->transform->set_position(0, 190, 50);
		SpotLight* s = spot_light->spot_light.create(game->mallocator);

		s->set_direction(-90, 0);
		s->cutoff = 30;
		s->set_diffuse_color(0, 255, 0);
		s->diffuse_intensity = 30;
		s->attenuation = .001;
		s->specular_intensity = 10;
		s->set_specular_color(0, 255, 0);


		
		//Point lights
		//Point light 1
		spheres[0].create(game->mallocator,*walls[3]);
		spheres[0]->model->set_color_and_flatness(255, 255, 255, 255);
		spheres[0]->model->cast_shadows = false;
		spheres[0]->model->set_texture_normal(white, white_normal);
		spheres[0]->transform->set_scale(12, 12, 12);
		spheres[0]->transform->set_position(180, 20, 180);
		spheres[0]->model->mesh = sphere_mesh;

		PointLight* p = spheres[0]->point_light.create(game->mallocator);
		p->set_diffuse_color(255, 80, 0);
		p->diffuse_intensity = 3;
		p->attenuation = .001;
		p->specular_intensity = 10;
		p->set_specular_color(255, 80, 0);

		//Point light 2
		spheres[1].create(game->mallocator, *spheres[0]);
		spheres[1]->transform->set_position(-180, 20, 180);

		//Point light 3
		spheres[2].create(game->mallocator, *spheres[1]);
		spheres[2]->transform->set_position(-180, 20, -180);

		//Point light 4
		spheres[3].create(game->mallocator, *spheres[2]);
		spheres[3]->transform->set_position(180, 20, -180);

		//Directional light
		directional_light.create(game->mallocator);
		DirectionalLight* l = directional_light->dir_light.create(game->mallocator);
		directional_light->transform.create(game->mallocator);
		l->set_direction(-65, 150);
		l->diffuse_intensity = 0.2;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = .05;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = .05;
		l->set_ambient_color(255, 255, 200);

		//GUI
		font.create(game->mallocator,"Aaargh", 24, GAME);
		gui.create(game->mallocator);
		
		Sprite* sp = gui->sprite.create(game->mallocator);
		Text* t = gui->text.create(game->mallocator);
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

		emitter_obj.create(game->mallocator);
		emitter_obj->transform.create(game->mallocator);
		emitter_obj->transform->set_position(0, 105, 50);
		Emitter* emitter = emitter_obj->emitter.create(game->mallocator,*emitter_obj);
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
			p->model.create(Mallocator::get_instance());
			p->model->set_texture_normal(white, normal);
			p->model->mesh = mesh;
			p->model->color = ColorRGBA::yellow;
			//p->set_emitter(new Emitter(emitter));
		};
		emitter->lambda_spawn = [](Emitter* e){
			for (U8 t = 0; t < 2; t++){
				Particle3D* p = e->new_particle();
				p->transform.create(Mallocator::get_instance());
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

		/*e2->lambda_creation = [](Emitter* e,Particle3D* p){
		Texture game->white, game->normal;
		I32 game->mesh;
		Emitter* game->emitter;
		e->save_map.get("texture", &game->white);
		e->save_map.get("normal", &game->normal);
		e->save_map.get("mesh", &game->mesh);
		p->speed = .95f;
		p->decay_rate = .005f;
		p->transform->scale = glm::vec3(10);
		p->model = new  Model();
		p->model->set_texture_normal(game->white, game->normal);
		p->model->mesh = game->mesh;
		p->model->color = ColorRGBA::red;
		};
		e2->lambda_spawn = [](Emitter* e){
		bool go_crazy;
		e->save_map.get("go_crazy", &go_crazy);
		if (go_crazy){
		for (U8 t = 0; t < 10; t++){
		Particle3D* p = e->new_particle();
		p->transform->position = e->game_object->transform->position;
		p->direction = ryno_math::get_rand_dir(0, 360, 0, 360);
		}
		}
		};
		e2->lambda_particle_update = [](Emitter* e, Particle3D* p, F32 game->delta_time)
		{
		p->transform->set_position(p->direction * p->speed * game->delta_time + p->transform->position);
		};*/


		//e2->init(200);
		emitter->init(200);
		


	}


	void ShowScene::update(){

		game->particle_manager->update(game->delta_time);
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


	void ShowScene::NewTerrain()
	{
		I32 m_SegmentCount = 80;
		F32 m_Length = 5;
		F32 m_Width = 5;
		F32 m_Height = 50;

		for (int i = 0; i < m_SegmentCount; i++)
		{
			float z = m_Length * i;
			float v = (1.0f / m_SegmentCount) * i;

			for (int j = 0; j < m_SegmentCount; j++)
			{
				float x = m_Width * j;
				float u = (1.0f / m_SegmentCount) * j;

				glm::vec3 offset;
				if (j == 0 || j == m_SegmentCount - 1 || i == 0 || i == m_SegmentCount - 1)
					offset = glm::vec3(x - m_Width * m_SegmentCount / 2.0f, 0, z - m_Length * m_SegmentCount / 2.0f);
				else
					offset = glm::vec3(x - m_Width * m_SegmentCount / 2.0f, m_Height * PerlinNoise::perlin(x, z, 1 / 80.0f), z - m_Length * m_SegmentCount / 2.0f);

				glm::vec2 uv = glm::vec2(u, v);
				bool buildTriangles = i > 0 && j > 0;

				BuildQuadForGrid(offset, uv, buildTriangles, m_SegmentCount);
			}
		}
		game->mesh_builder->calculate_tangents();
		game->mesh_builder->calculate_normals();


	}

	void ShowScene::BuildQuadForGrid(glm::vec3 position, glm::vec2 uv,
		bool buildTriangles, int vertsPerRow)
	{
		game->mesh_builder->new_vertex();
		game->mesh_builder->set_position(position);
		game->mesh_builder->set_uvs(uv);

		if (buildTriangles)
		{
			int baseIndex = game->mesh_builder->get_vertices_count() - 1;

			int index0 = baseIndex;
			int index1 = baseIndex - 1;
			int index2 = baseIndex - vertsPerRow;
			int index3 = baseIndex - vertsPerRow - 1;

			game->mesh_builder->add_triangle(index0, index2, index1);
			game->mesh_builder->add_triangle(index2, index3, index1);
		}
	}

}