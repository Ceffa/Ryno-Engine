#include "MainGame.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Text.h"
#include "GPUProfiler.h"
#include "CPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>





namespace Ryno{

	void MainGame::start(){
		CPUProfiler::begin();
		CPUProfiler::start_time();

		//initializations
		m_camera->position = glm::vec4(0,150,-1500, 1);
		

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
		I32 cube_mesh = m_mesh_manager->load_mesh("cube", 1, GAME_FOLDER);
		I32 sphere_model = m_mesh_manager->load_mesh("sphere", 1, GAME_FOLDER);
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

		//Cone
		//GameObject* go = new GameObject();
		//go->model->set_color_and_flatness(255, 255, 255, 0);
		//go->model->set_texture_normal(white, white_normal);
		//go->model->mesh = cone_mesh;
		//go->scale = glm::vec3(100,100,100);
		//go->position = glm::vec3(0, 55, 0);
		//
	
		//go = new GameObject(go);
		//go->scale = glm::vec3(20, 20, 20);
		//go->position = glm::vec3(-150, 15, -170);

		//go = new GameObject(go);
		//go->position = glm::vec3(150, 15, -170);


		////Base
		//go = new GameObject(go);
		//go->scale = glm::vec3(400, 10, 400);
		//go->model->set_texture_normal(bt, bn);

		//go->model->mesh = cube_mesh;
		//go->model->set_tiling(3, 3);
		//go->position = glm::vec3(0, 0, 0);
		////Left
		//go = new GameObject(go);
		//go->scale = glm::vec3(10, 200, 400);
		//go->position = glm::vec3(-200, 105, 0);
		////Right
		//go = new GameObject(go);
		//go->scale = glm::vec3(10, 200, 400);
		//go->position = glm::vec3(200, 105, 0);
		////Front
		//go = new GameObject(go);
		//go->scale = glm::vec3(400, 200, 10);
		//go->position = glm::vec3(0, 105, 200);
		////Roof
		//go = new GameObject(go);
		//go->scale = glm::vec3(400, 10, 400);
		//go->position = glm::vec3(0, 205, 0);
		//Sphere
		GameObject* go = new GameObject();
		go->scale = glm::vec3(100,100,100);
		go->model->mesh = sphere_model;
		go->model->texture = white;
		go->position = glm::vec3(0, 300, 0);
		go->model->set_color_and_flatness(255, 255, 0,255);
		go->model->cast_shadows = false;
		ball = go;


		CPUProfiler::next_time();

		//build light

		GameObject* bl;
		//PointLight* p;
		
		/*SpotLight* s = new SpotLight();
		s->set_position(0,190,0);
		s->set_direction(-90,0);
		s->cutoff = 40;
		s->set_diffuse_color(0,255,0);
		s->diffuse_intensity = 30;
		s->attenuation = .001;
		s->specular_intensity = 10;
		s->set_specular_color(0,255,0);

		PointLight* p = new PointLight();
		p->set_position(180, 20, 180);
		p->set_diffuse_color(255, 80, 0);
		p->diffuse_intensity = 3;
		p->attenuation = .001;
		p->specular_intensity = 10;
		p->set_specular_color(255, 80, 0);

		go = new GameObject(go);
		go->model->set_color_and_flatness(255, 255, 255, 255);
		go->model->cast_shadows = false;
		go->model->set_texture_normal(white, white_normal);
		go->scale = glm::vec3(25,25,25);
		go->position = glm::vec3(180, 20, 180);
		spheres[0] = go;
		
		p = new PointLight(p);
		p->set_position(-180, 20, 180);

		go = new GameObject(go);
		go->position = glm::vec3(-180, 20, 180);
		spheres[1] = go;

		p = new PointLight(p);
		p->set_position(-180, 20, -180);

		go = new GameObject(go);
		go->position = glm::vec3(-180, 20, -180);
		spheres[2] = go;

		p = new PointLight(p);
		p->set_position(180, 20, -180);

		go = new GameObject(go);
		go->position = glm::vec3(180, 20, -180);
		spheres[3] = go;*/

		DirectionalLight* l = new DirectionalLight();
		l->set_direction(-65, 150);
		l->diffuse_intensity = 0.2;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = .05;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = .05;
		l->set_ambient_color(255, 255, 200);
		
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

		
		
		CPUProfiler::end_time();
		CPUProfiler::print_time();


		set_physics();
		

		
		
	}
	
	

	void MainGame::update(){

		static I32 cnt = 0;

		if (shell->restart_physics){
			shell->restart_physics = false;
			set_physics();
		}
		if (!shell->active || shell->phys_step){
			glm::vec3 force = glm::vec3(0, 0, 0);
			F32 time_step = delta_time * 0.001; //from ms to s
			

			glm::vec3 F0 = Fw.get_force();
			glm::vec3 F1 = Fd.get_force();
		

	
		

			force =  F0 + F1;
	
			glm::vec3 old_a = acceleration;
			ball->position += 100.0f * (velocity * time_step + (0.5f * old_a * time_step * time_step));
			acceleration = force / Fw.m;
		
			//Log::println("Old v: "); //Log::print(velocity.x);
			//Log::print(" Old a: "); //Log::print(old_a.x);
			//Log::print(" F: "); //Log::print(force.x);
			//Log::print(" a: "); //Log::print(acceleration.x);
		

			glm::vec3 avg_a = 0.5f * ( acceleration + old_a);
		
			velocity += avg_a * time_step;
			//Log::print(" avga: "); //Log::print(avg_a.x);
			//Log::print(" v: "); //Log::print(velocity.x);
			

			Fd.v = glm::length(velocity);
			Fd.dir = glm::normalize(velocity);

			//Collision
			if (ball->position.y - ball->scale.y < -100 && velocity.y < 0)
			{
				/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
				velocity.y *= -0.8;
				/* Move the ball back a little bit so it's not still "stuck" in the wall. */
				ball->position.y = -100 + ball->scale.y;
			}
			if (ball->position.x + ball->scale.x > 500 && velocity.x > 0)
			{
				/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
				velocity.x *= -0.8;
				/* Move the ball back a little bit so it's not still "stuck" in the wall. */
				ball->position.x = 500 - ball->scale.x;
			}
			else if (ball->position.x - ball->scale.x < -500 && velocity.x < 0)
			{
				/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
				velocity.x *= -0.8;
				/* Move the ball back a little bit so it's not still "stuck" in the wall. */
				ball->position.x = -500 + ball->scale.x;
			}


			shell->phys_step = false;


			if (++cnt >= 2){
				cnt = 0;
				for (GameObject* o : markers){
					if (o->model->color.g >0) o->model->color.g--;
				}
				GameObject* go = new GameObject(marker);
				go->model->set_color_and_flatness(255, 255, 0, 255);
				go->position = ball->position;
				markers.push_back(go);
			}
			


		}


	}

	
	void MainGame::set_physics()
	{

		ball->position = glm::vec3(0, 300, 0);
		velocity = glm::vec3(4,10, 0);

		for (GameObject* go : markers)
			delete go;
		markers.clear();

		Fw.a = glm::vec3(0, -9.8, 0);
		Fw.m = 100;


		Fd.A = M_PI * ball->scale.x * ball->scale.x / 10000.0f;
		Fd.Cd = 0.47f;
		Fd.p = 1.2f;
	
	
		marker = new GameObject();
		marker->scale = glm::vec3(8,8,8);
		marker->model->mesh = marker_mesh;
		marker->model->texture = white;
		marker->position = ball->position;
		marker->model->set_color_and_flatness(255, 255, 0, 255);
		marker->model->cast_shadows = false;
		markers.push_back(marker);
	}

	void MainGame::input(){
		if (!shell->active){
			if (m_input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				sound.play();

			}
		




			if (m_input_manager->is_key_down(SDLK_LEFT, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->position.x -= 0.5f * delta_time;

				}
				for (GameObject* l : spheres){
					l->position.x -= 0.5f * delta_time;

				}
				for (PointLight* l : PointLight::point_lights){
					l->position.x -= 0.5f * delta_time;

				}

			}
			if (m_input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->position.x += 0.5f * delta_time;

				}
				for (GameObject* l : spheres){
					l->position.x += 0.5f * delta_time;

				}
				for (PointLight* l : PointLight::point_lights){
					l->position.x += 0.5f * delta_time;

				}

			}
			if (m_input_manager->is_key_down(SDLK_b, KEYBOARD)){
				static float a = 0;
				static float b = 0;
				a += 0.5;
				b += 0.5;

				DirectionalLight::directional_light->set_direction(-45, 180);

			}


			if (m_input_manager->is_key_down(SDLK_n, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->position.z -= 0.5f * delta_time;

				}
				for (GameObject* l : spheres){
					l->position.z -= 0.5f * delta_time;

				}
				for (PointLight* l : PointLight::point_lights){
					l->position.z -= 0.5f * delta_time;

				}


			}
			if (m_input_manager->is_key_down(SDLK_m, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->position.z += 0.5f * delta_time;

				}
				for (GameObject* l : spheres){
					l->position.z += 0.5f * delta_time;

				}
				for (PointLight* l : PointLight::point_lights){
					l->position.z += 0.5f * delta_time;

				}

			}
			if (m_input_manager->is_key_down(SDLK_UP, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->position.y += 0.5f * delta_time;

				}

				for (GameObject* l : spheres){
					l->position.y += 0.5f * delta_time;

				}
				for (PointLight* l : PointLight::point_lights){
					l->position.y += 0.5f * delta_time;

				}

			}
			if (m_input_manager->is_key_down(SDLK_DOWN, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->position.y -= 0.5f * delta_time;

				}
				for (GameObject* l : spheres){
					l->position.y -= 0.5f * delta_time;

				}
				for (PointLight* l : PointLight::point_lights){
					l->position.y -= 0.5f * delta_time;

				}


			}
			if (m_input_manager->is_key_down(SDLK_o, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){

					l->diffuse_intensity += 0.1f;

				}
				for (PointLight* l : PointLight::point_lights){
					l->diffuse_intensity += 0.1;

				}
			}
			if (m_input_manager->is_key_down(SDLK_p, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					if (l->diffuse_intensity > 0)
						l->diffuse_intensity -= 1;

				}
				for (PointLight* l : PointLight::point_lights){
					if (l->diffuse_intensity > 0)
						l->diffuse_intensity -= 1;

				}
			}
			if (m_input_manager->is_key_down(SDLK_k, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->specular_intensity += .3;

				}
				for (PointLight* l : PointLight::point_lights){
					l->specular_intensity += .3;

				}
			}
			if (m_input_manager->is_key_down(SDLK_l, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					if (l->specular_intensity > 0)
						l->specular_intensity -= .3;

				}
				for (PointLight* l : PointLight::point_lights){
					if (l->specular_intensity > 0)
						l->specular_intensity -= .3;

				}
			}

			if (m_input_manager->is_key_down(SDLK_1, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->cutoff -= 1;
					if (l->cutoff < 5)
						l->cutoff = 5;
				}
			}

			if (m_input_manager->is_key_down(SDLK_2, KEYBOARD)){
				for (SpotLight* l : SpotLight::spot_lights){
					l->cutoff += 1;
					if (l->cutoff > 60)
						l->cutoff = 60;
				}
			}


		}
		

	}
	
}