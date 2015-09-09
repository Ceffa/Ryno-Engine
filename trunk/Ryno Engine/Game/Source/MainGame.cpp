#include "MainGame.h"
#include "DirectionalLight.h"
#include "PointLight.h"
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
		m_camera->position = glm::vec4(0,150,-500, 1);
		

		CPUProfiler::next_time();

		//loading graphics
		Texture white = m_texture_manager->loadPNG("white_pixel",GAME_FOLDER);
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
		cube_mesh = m_mesh_manager->load_mesh("cube", 1, GAME_FOLDER);
		I32 sphere_model = m_mesh_manager->load_mesh("sphere", 1, GAME_FOLDER);
		I32 cone_mesh = m_mesh_manager->load_mesh("cone", 1, GAME_FOLDER);

		CPUProfiler::next_time();

		//loading skyboxes
		m_camera->skybox = m_texture_manager->loadCubeMap("full_moon_small", GAME_FOLDER);

		CPUProfiler::next_time();

		//loading audio
		sound = m_audio_manager.load_sound("stomp.wav", GAME_FOLDER);
		music = m_audio_manager.load_music("cthulhu.ogg", GAME_FOLDER);
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();


		CPUProfiler::next_time();

		



		sphere_box_model = new Model();
		sphere_box_model->mesh = bound_sphere;


		CPUProfiler::next_time();

		//Build the environnement

		//Cone
		GameObject* go = new GameObject();
		go->model->set_color_and_flatness(255, 255, 255, 0);
		go->model->set_texture_normal(white, white_normal);
		go->model->mesh = cone_mesh;
		go->scale = glm::vec3(100,100,100);
		go->position = glm::vec3(0, 55, 0);
		
	
		go = new GameObject(go);
		go->scale = glm::vec3(20, 20, 20);
		go->position = glm::vec3(-150, 15, -170);

		go = new GameObject(go);
		go->position = glm::vec3(150, 15, -170);


		//Base
		go = new GameObject(go);
		go->scale = glm::vec3(400, 10, 400);
		go->model->set_texture_normal(bt, bn);

		go->model->mesh = cube_mesh;
		go->model->set_tiling(3, 3);
		go->position = glm::vec3(0, 0, 0);
		//Left
		go = new GameObject(go);
		go->scale = glm::vec3(10, 200, 400);
		go->position = glm::vec3(-200, 105, 0);
		//Right
		go = new GameObject(go);
		go->scale = glm::vec3(10, 200, 400);
		go->position = glm::vec3(200, 105, 0);
		//Front
		go = new GameObject(go);
		go->scale = glm::vec3(400, 200, 10);
		go->position = glm::vec3(0, 105, 200);
		//Roof
		go = new GameObject(go);
		go->scale = glm::vec3(400, 10, 400);
		go->position = glm::vec3(0, 205, 0);
		//Sphere
		go = new GameObject(go);
		go->scale = glm::vec3(100,100,100);
		go->model->mesh = sphere_model;
		go->position = glm::vec3(0, 300, -150);


		CPUProfiler::next_time();

		//build light

		GameObject* bl;
		//PointLight* p;
		


	
		SpotLight* s = new SpotLight();
		s->set_position(0,190,0);
		s->set_direction(-90, 0);
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
		
		p = new PointLight(p);
		p->set_position(-180, 20, 180);

		go = new GameObject(go);
		go->position = glm::vec3(-180, 20, 180);

		p = new PointLight(p);
		p->set_position(-180, 20, -180);

		go = new GameObject(go);
		go->position = glm::vec3(-180, 20, -180);

		p = new PointLight(p);
		p->set_position(180, 20, -180);

		go = new GameObject(go);
		go->position = glm::vec3(180, 20, -180);


		DirectionalLight* l = new DirectionalLight();
		l->set_direction(-65, 150);
		l->diffuse_intensity = 0.2;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = .05;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = .05;
		l->set_ambient_color(255, 255, 200);


		Sprite* sp = new Sprite();
		sp->depth = 2;
		sp->angle = 0;
		sp->color.set_color_and_alpha(255, 255, 255, 255);
		sp->set_texture(doge);
		
		sp->position = glm::vec2(0.25,0.75);
		sp->anchor_point = CENTER;
		sp->scale = glm::vec2(150,150);
		sp->tiling = glm::vec2(1, 1);
		sp->angle = 0;

		sp = new Sprite(sp);
		sp->position = glm::vec2(0.75,0.75);
		sp->scale = glm::vec2(-150, 150);

		sp = new Sprite(sp);
		sp->position = glm::vec2(0.25, 0.25);
		sp->scale = glm::vec2(150,-150);

		sp = new Sprite(sp);
		sp->position = glm::vec2(0.75, 0.25);
		sp->scale = glm::vec2(-150,-150);
	
		Font* f = new Font("Aaargh",24);
		Text* t = new Text();
		t->depth = 1;
		t->font = f;
		t->position = glm::vec2(1,1);

		t->anchor_point = TOP_RIGHT;
		t->scale = glm::vec2(1,1);
		t->color = ColorRGBA(255,255,0, 255);
		t->text = "So much text.\nVery Doge.\nWOW";

		t = new Text(t);
		t->position = glm::vec2(0, 1);
		t->anchor_point = TOP_LEFT;

		t = new Text(t);
		t->position = glm::vec2(.5, 1);
		t->anchor_point = TOP_MIDDLE;

		t = new Text(t);
		t->position = glm::vec2(1, 0);
		t->anchor_point = BOTTOM_RIGHT;
		t->text = "Wow\nMuch aligned.\nSo very Wow.";

		t = new Text(t);
		t->position = glm::vec2(0, 0);
		t->anchor_point = BOTTOM_LEFT;

		t = new Text(t);
		t->position = glm::vec2(.5, 0);
		t->anchor_point = BOTTOM_MIDDLE;

		t = new Text(t);
		t->position = glm::vec2(1, .5);
		t->anchor_point = MIDDLE_RIGHT;
		t->text = "Much Formatted\nwow\nSo Much Doge.\nVery Wow.";
		
		t = new Text(t);
		t->position = glm::vec2(0, .5);
		t->anchor_point = MIDDLE_LEFT;
		t = new Text(t);
		t->position = glm::vec2(.5, .5);
		t->anchor_point = CENTER;
		
		CPUProfiler::end_time();
		CPUProfiler::print_time();



	}
	
	void MainGame::input(){

		if (m_input_manager.is_key_down(SDLK_d, KEYBOARD)){
			m_camera->move_right(3.0f);

		}
		if (m_input_manager.is_key_down(SDLK_a, KEYBOARD)){
			m_camera->move_left(3.0f);
		}
		if (m_input_manager.is_key_down(SDLK_w, KEYBOARD) || m_input_manager.is_key_down(SDL_BUTTON_LEFT, MOUSE)){
			m_camera->move_forward(3.0f);
		}
		if (m_input_manager.is_key_down(SDLK_s, KEYBOARD) || m_input_manager.is_key_down(SDL_BUTTON_RIGHT, MOUSE)){
			m_camera->move_back(3.0f);
		}
		
	
	
		
		if (m_input_manager.is_key_down(SDLK_LEFT, KEYBOARD) || m_input_manager.is_key_down(SDL_CONTROLLER_BUTTON_DPAD_LEFT, CONTROLLER)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->position.x -= 3;

			}
			for (PointLight* l : PointLight::point_lights){
				l->position.x -= 3;

			}
		
		}
		if (m_input_manager.is_key_down(SDLK_RIGHT, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->position.x += 3;

			}
			for (PointLight* l : PointLight::point_lights){
				l->position.x += 3;

			}
		
		}
		if (m_input_manager.is_key_down(SDLK_b, KEYBOARD)){
			static float a = 0;
			static float b = 0;
			a += 0.5;
			b += 0.5;
			
			DirectionalLight::directional_light->set_direction(-45,180);
			
		}


		if (m_input_manager.is_key_down(SDLK_n, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->position.z -= 3;

			}
			for (PointLight* l : PointLight::point_lights){
				l->position.z -= 3;

			}

		
		}
		if (m_input_manager.is_key_down(SDLK_m, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->position.z += 3;

			}
			for (PointLight* l : PointLight::point_lights){
				l->position.z += 3;

			}
		
		}
		if (m_input_manager.is_key_down(SDLK_UP, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->position.y += 3;

			}
			for (PointLight* l : PointLight::point_lights){
				l->position.y += 3;

			}
			
		}
		if (m_input_manager.is_key_down(SDLK_DOWN, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->position.y -= 3;

			}
			for (PointLight* l : PointLight::point_lights){
				l->position.y -= 3;

			}

		
		}
		if (m_input_manager.is_key_down(SDL_CONTROLLER_BUTTON_Y, CONTROLLER)){
			for (SpotLight* l : SpotLight::spot_lights){
				
				l->diffuse_intensity += 1;
				
			}
			for (PointLight* l : PointLight::point_lights){
				l->diffuse_intensity += 1;

			}
		}
		if (m_input_manager.is_key_down(SDLK_p, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				if (l->diffuse_intensity > 0)
				l->diffuse_intensity -= 1;

			}
			for (PointLight* l : PointLight::point_lights){
				if (l->diffuse_intensity > 0)
					l->diffuse_intensity -= 1;

			}
		}
		if (m_input_manager.is_key_down(SDLK_k, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->specular_intensity += .3;

			}
			for (PointLight* l : PointLight::point_lights){
				l->specular_intensity += .3;

			}
		}
		if (m_input_manager.is_key_down(SDLK_l, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				if (l->specular_intensity > 0)
				l->specular_intensity -= .3;

			}
			for (PointLight* l : PointLight::point_lights){
				if (l->specular_intensity > 0)
					l->specular_intensity -= .3;

			}
		}

		if (m_input_manager.is_key_down(SDLK_1, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->cutoff -= 1;
				if (l->cutoff < 5)
					l->cutoff = 5;
			}
		}

		if (m_input_manager.is_key_down(SDLK_2, KEYBOARD)){
			for (SpotLight* l : SpotLight::spot_lights){
				l->cutoff += 1;
				if (l->cutoff > 60)
					l->cutoff = 60;
			}
		}



		glm::vec2 mouse_coords = m_input_manager.get_mouse_movement();
		m_camera->rotate(0.005f * mouse_coords.x, 0.005f* mouse_coords.y);
		glm::vec2 rotation_view = m_input_manager.get_controller_RX_coords();
		m_camera->rotate(0.05f * rotation_view.x, 0.05f* rotation_view.y);
		glm::vec2 direction = m_input_manager.get_controller_LX_coords();
		m_camera->move_forward(5.0f * -direction.y);
		m_camera->move_right(5.0f * direction.x);

	}

	void MainGame::update(){
		

	}

	void MainGame::draw(){

		m_deferred_renderer->init_frame();
		m_deferred_renderer->geometry_pass();
		m_deferred_renderer->prepare_for_light_passes();
		m_deferred_renderer->spot_light_pass();
		m_deferred_renderer->point_light_pass();
		m_deferred_renderer->directional_light_pass();
		m_deferred_renderer->skybox_pass();
		m_deferred_renderer->draw_HUD_pass();
		m_deferred_renderer->final_pass();

		SDL_GL_SwapWindow(m_window);

		

		
	
	

	}

	void MainGame::end(){
		
		MainGameInterface::end();
		
	}
}