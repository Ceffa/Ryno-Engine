#include "MainGame.h"
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

namespace Ryno{

	void MainGame::start(){
		CPUProfiler::begin();
		CPUProfiler::start_time();

		//initializations
		m_camera->position = glm::vec4(0,150,-500, 1);

		CPUProfiler::next_time();

		//loading graphics
		white = m_texture_manager->loadPNG("white_pixel",GAME_FOLDER);
		Texture white_normal = m_texture_manager->loadPNG("normal_pixel", GAME_FOLDER);
		Texture texture_bricks = m_texture_manager->loadPNG("pack/177", GAME_FOLDER);
		Texture normal_map_bricks = m_texture_manager->loadPNG("pack/177_norm", GAME_FOLDER);
		Texture at = m_texture_manager->loadPNG("pack/196", GAME_FOLDER);
		Texture an = m_texture_manager->loadPNG("pack/196_norm", GAME_FOLDER);
		Texture bt = m_texture_manager->loadPNG("pack/161", GAME_FOLDER);
		Texture bn = m_texture_manager->loadPNG("pack/161_norm", GAME_FOLDER);
		Texture solaire = m_texture_manager->loadPNG("solaire", GAME_FOLDER);
		Texture sun = m_texture_manager->loadPNG("sun", GAME_FOLDER);
		Texture doge = m_texture_manager->loadPNG("doge", GAME_FOLDER);

		CPUProfiler::next_time();

		//loading models
		static I32 cube_mesh = m_mesh_manager->load_mesh("cube", 1, GAME_FOLDER);
		static I32 sphere_mesh = m_mesh_manager->load_mesh("sphere", 1, GAME_FOLDER);
		I32 cone_mesh = m_mesh_manager->load_mesh("cone", 1, GAME_FOLDER);
		marker_mesh = m_mesh_manager->load_mesh("marker", 1, GAME_FOLDER);

		CPUProfiler::next_time();

		//loading skyboxes
		m_camera->skybox = m_texture_manager->loadCubeMap("full_moon_small", GAME_FOLDER);

		CPUProfiler::next_time();

		//loading audio
		sound = m_audio_manager->load_sound("stomp.wav", GAME_FOLDER);
		music = m_audio_manager->load_music("journey.ogg", GAME_FOLDER);
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();
	

		CPUProfiler::next_time();

		//Build the environnement

		//Cones
		GameObject* go = new GameObject();
		go->model = new Model();
		go->model->set_color_and_flatness(255, 255, 255, 0);
		go->model->set_texture_normal(white, white_normal);
		go->model->mesh = cone_mesh;
		go->transform->set_scale(20,20,20);
		go->transform->set_position(0, 15, 50);
		
	
		go = new GameObject(go);
		go->transform->set_scale(20, 20, 20);
		go->transform->set_position(-150, 15, -170);

		go = new GameObject(go);
		go->transform->set_position(150, 15, -170);


		//Base
		go = new GameObject(go);
		go->transform->set_scale(200, 5, 200);
		go->model->set_texture_normal(bt, bn);

		go->model->mesh = cube_mesh;
		go->model->set_tiling(3, 3);
		go->transform->set_position(0, 0, 0);
		//Left
		go = new GameObject(go);
		go->transform->set_scale(5, 100, 200);
		go->transform->set_position(-200, 105, 0);
		//Right
		go = new GameObject(go);
		go->transform->set_scale(5, 100, 200);
		go->transform->set_position(200, 105, 0);
		//Front
		go = new GameObject(go);
		go->transform->set_scale(200, 100, 5);
		go->transform->set_position(0, 105, 200);
		//Roof
		go = new GameObject(go);
		go->transform->set_scale(200, 5, 200);
		go->transform->set_position(0, 205, 0);
	
		SpotLight* s = new SpotLight();
		s->set_direction(-90, 0);
		s->cutoff = 30;
		s->set_diffuse_color(0, 255, 0);
		s->diffuse_intensity = 30;
		s->attenuation = .001;
		s->specular_intensity = 10;
		s->set_specular_color(0, 255, 0);

		GameObject* spot_light = new GameObject();
		spot_light->transform->set_position(0, 190, 50);
		spot_light->spot_light = s;
		spheres[4] = spot_light;

		PointLight* p = new PointLight();
		p->set_diffuse_color(255, 80, 0);
		p->diffuse_intensity = 3;
		p->attenuation = .001;
		p->specular_intensity = 10;
		p->set_specular_color(255, 80, 0);

		go = new GameObject(go);
		go->model->set_color_and_flatness(255, 255, 255, 255);
		go->model->cast_shadows = false;
		go->model->set_texture_normal(white, white_normal);
		go->transform->set_scale(12, 12, 12);
		go->transform->set_position(180, 20, 180);
		go->model->mesh = sphere_mesh;
		go->point_light = p;
		spheres[0] = go;


		go = new GameObject(go);
		go->transform->set_position(-180, 20, 180);
		go->point_light = new PointLight(p);
		spheres[1] = go;


		go = new GameObject(go);
		go->transform->set_position(-180, 20, -180);
		go->point_light = new PointLight(p);
		spheres[2] = go;

	
		go = new GameObject(go);
		go->transform->set_position(180, 20, -180);
		go->point_light = new PointLight(p);
		spheres[3] = go;

		




	
		CPUProfiler::next_time();

		//build light

		GameObject* bl;
	
		
	

		
		DirectionalLight* l = new DirectionalLight();
		l->set_direction(-65, 150);
		l->diffuse_intensity = 0.2;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = .05;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = .05;
		l->set_ambient_color(255, 255, 200);
		GameObject* dir_light = new GameObject();
		dir_light->dir_light = l;
	

		
		/*Sprite* sp = new Sprite();
		sp->depth = 20;
		sp->angle = 0;
		sp->set_color(255, 255, 255, 255);
		sp->set_texture(doge);
		sp->set_position(0.25,0.75);
		sp->anchor_point = CENTER;
		sp->set_scale(150, 150);
		sp->set_tiling(1, 1);
		sp->angle = 0;


		Font* f = new Font("Aaargh",24, GAME_FOLDER);
		Text* t = new Text();
		t->depth = 10;
		t->font = f;
		t->set_position(1,1);
		t->anchor_point = TOP_RIGHT;
		t->set_scale(1,1);
		t->set_color(255,255,0, 255);
		t->text = "Ryno Engine";*/

		GameObject* shos = new GameObject();
		shos->model = new Model();
		shos->model->set_texture_normal(white, white_normal);
		shos->model->mesh = cube_mesh;
		shos->model->cast_shadows = true;
		shos->active = true;

		Emitter* emitter = new Emitter();
		particle_batch = new GameObject();
		emitter->init(20000, shos);
		
		/*emitter->lambda_particle = [](Particle3D* p, float delta_time)
		{
			p->transform->set_position(p->direction * p->speed * delta_time + p->transform->position);
			p->model->color = lerp(ColorRGBA(0, 255, 0, 255), ColorRGBA(0, 0,255, 255), p->lifetime * 1.0f);
			p->transform->scale = glm::vec3(lerp(1, 50, p->lifetime));
			
			if (p->lifetime > 0.6 && p->model->mesh == cube_mesh){
				p->model->mesh = sphere_mesh;
			}
		};
			

		emitter->lambda_emitter = [](Emitter* e, F32 delta_time){
			for (U32 i = 0; i < 5; i++){
				
				Particle3D* p = e->new_particle();
				p->transform->position = e->game_object->transform->position;
				p->speed = .25f;
				p->decay_rate = .0005f;
				p->direction = get_particle_dir(0,180,0,180);
				p->model->mesh = cube_mesh;
			}
		};
*/

		emitter->lambda_particle = [](Particle3D* p, float delta_time)
		{
			p->transform->set_position(p->direction * p->speed * delta_time + p->transform->position);
			p->model->color = ryno_math::lerp(ColorRGBA(0, 255, 0, 255), ColorRGBA(0, 0, 255, 255), p->lifetime * 1.0f);
			p->transform->scale = glm::vec3(ryno_math::lerp(1, 50, p->lifetime));

			if (p->lifetime > 0.6 && p->model->mesh == cube_mesh){
				p->model->mesh = sphere_mesh;
			}
		};


		emitter->lambda_emitter = [](Emitter* e, F32 delta_time){
			for (U32 i = 0; i < 5; i++){

				Particle3D* p = e->new_particle();
				p->transform->position = e->game_object->transform->position;
				p->speed = .25f;
				p->decay_rate = .0005f;
				p->direction = ryno_math::get_rand_dir(0, 180, 0, 180);
				p->model->mesh = cube_mesh;
			}
		};


		particle_batch->set_emitter(emitter);
		
		
		CPUProfiler::end_time();
		CPUProfiler::print_time();

	}
	
	

	void MainGame::update(){

		m_particle_manager->update(delta_time);
		for (I32 i = 0; i < 4; i++){
			spheres[i]->transform->position.y = 40 + sin(m_time_manager->current_time / 1000.0f) * 20;
		}

	}
	
	void MainGame::input(){
		if (!shell->active){
			if (m_input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				sound.play();

			}
			if (m_input_manager->is_key_down(SDLK_LEFT, KEYBOARD)){
				particle_batch->transform->add_position(-.5f* delta_time, 0, 0);
			}
			if (m_input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)){
				particle_batch->transform->add_position(.5f * delta_time , 0, 0);
			}
			if (m_input_manager->is_key_down(SDLK_UP, KEYBOARD)){
				particle_batch->transform->add_position(0, .5f* delta_time, 0);
			}
			if (m_input_manager->is_key_down(SDLK_DOWN, KEYBOARD)){
				particle_batch->transform->add_position(0, -.5f* delta_time, 0);
			}
			if (m_input_manager->is_key_down(SDLK_n, KEYBOARD)){
				particle_batch->transform->add_position(0, 0, .5f* delta_time);
			}
			if (m_input_manager->is_key_down(SDLK_m, KEYBOARD)){
				particle_batch->transform->add_position(0, 0, -.5f* delta_time);
			}
		}
	}
	
}