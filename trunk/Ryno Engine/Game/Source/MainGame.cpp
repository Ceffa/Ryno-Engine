#include "MainGame.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "GPUProfiler.h"
#include "CPUProfiler.h"
#include <GLM/gtx/transform.hpp>





namespace Ryno{

	void MainGame::start(){
		CPUProfiler::begin();
		CPUProfiler::start_time();

		//initializations
		m_camera->position = glm::vec4(0, 30, -50, 1);
		

		CPUProfiler::next_time();

		//loading graphics
		U32 white = m_texture_manager->loadPNG("white_pixel",GAME_FOLDER);
		U32 white_normal = m_texture_manager->loadPNG("normal_pixel", GAME_FOLDER);
		U32 texture_bricks = m_texture_manager->loadPNG("pack/177", GAME_FOLDER);
		U32 normal_map_bricks = m_texture_manager->loadPNG("pack/177_norm", GAME_FOLDER);
		U32 texture_red_wall = m_texture_manager->loadPNG("pack/178", GAME_FOLDER);
		U32 normal_map_red_wall = m_texture_manager->loadPNG("pack/178_norm", GAME_FOLDER);
		U32 texture_wood = m_texture_manager->loadPNG("pack/196", GAME_FOLDER);
		U32 normal_map_wood = m_texture_manager->loadPNG("pack/196_norm", GAME_FOLDER);

		CPUProfiler::next_time();

		//loading models
		cube_mesh = m_mesh_manager->load_mesh("cube", 1, GAME_FOLDER);
		I32 sphere_model = m_mesh_manager->load_mesh("sphere", 1, GAME_FOLDER);

		CPUProfiler::next_time();

		//loading skyboxes
		m_camera->skybox = m_texture_manager->loadCubeMap("full_moon", GAME_FOLDER);

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
		//Build the environnement
		GameObject* base = new GameObject();
		base->model.set_color_and_flatness(255, 255, 255, 0);
		base->model.set_mesh_texture_normal(cube_mesh, texture_bricks, normal_map_bricks);
		base->scale = glm::vec3(1000, 10, 1000);
		base->model.set_tiling(8, 8);
		base->position = glm::vec3(0, 0, 0);
		m_game_objects.push_back(base);

		/*GameObject* back = new GameObject();
		back->model.set_mesh_texture_normal(sphere_model, texture_bricks, normal_map_bricks);
		back->scale = glm::vec3(50, 50, 50);
		back->model.set_tiling(1, 1);
		back->position = glm::vec3(0, 70, 100);

		m_game_objects.push_back(back);

		GameObject* left = new GameObject();
		left->model.set_mesh_texture_normal(cube_mesh, texture_red_wall, normal_map_red_wall);
		left->scale = glm::vec3(40,40,40);
		left->position = glm::vec3(-200,25,120);
		left->rotate(10, 0, 0);
		left->model.set_tiling(1,1);
		m_game_objects.push_back(left);

		GameObject* right = new GameObject();
		right->model.set_mesh_texture_normal(cube_mesh, texture_wood, normal_map_wood);
		right->scale = glm::vec3(200, 50, 50);
		right->model.set_tiling(4,1);
		right->position = glm::vec3(-100, 50, 200);

		m_game_objects.push_back(right);*/

		/*go1 = new GameObject();
		go1->model.set_mesh_texture_normal(cube_mesh, texture_wood, normal_map_wood);
		go1->scale = glm::vec3(50, 200, 50);
		go1->model.set_tiling(1, 4);
		go1->position = glm::vec3(0, 100, 0);

		m_game_objects.push_back(go1);*/


		CPUProfiler::next_time();

		//build light

		GameObject* bl;
		//PointLight* p;
		


	
				p = new SpotLight();
				p->set_position(0, 10, 0);
				p->set_direction(0, 0);
				p->cutoff = 60;//no more then 0.5
				p->set_diffuse_color(255, 200, 0);
				p->diffuse_intensity = 30;
				p->attenuation = .1;
				p->specular_intensity = 1;
				p->set_specular_color(255, 200, 0);
				spot_lights.push_back(p);

				s = new PointLight();
				s->set_position(100, 10, 100);
				s->set_diffuse_color(255, 200, 0);
				s->diffuse_intensity = 30;
				s->attenuation = .1;
				s->specular_intensity = 1;
				s->set_specular_color(255, 200, 0);
				point_lights.push_back(s);
				
				l = new DirectionalLight();
				l->set_direction(-45, 45);
				l->diffuse_intensity = 0.5;
				l->set_diffuse_color(255, 200, 0);
				l->specular_intensity = .1;
				l->set_specular_color(255, 200, 0);
				l->ambient_intensity = .1;
				l->set_ambient_color(255, 200, 0);


		
		for (I8 i = 2; i < 4; i++){
			for (I8 j = 2; j < 4; j++){

				GameObject* n = new GameObject();
				n->model.set_mesh_texture_normal(cube_mesh, texture_wood, normal_map_wood);
				n->scale = glm::vec3(10, 150, 10);
				n->model.set_tiling(1, 4);
				n->position = glm::vec3(-500, 0, -500) +  glm::vec3(80 * i, 75, 80 * j);

				m_game_objects.push_back(n);
			}
		}

		for (I8 i = 2; i < 4; i++){
			for (I8 j = 2; j < 4; j++){

				GameObject* n = new GameObject();
				n->model.set_mesh_texture_normal(cube_mesh, texture_wood, normal_map_wood);
				n->scale = glm::vec3(10, 150, 10);
				n->model.set_tiling(1, 4);
				n->position =glm::vec3(-200,0,-200)+ glm::vec3(80 * i, 75, 80 * j);

				m_game_objects.push_back(n);
			}
		}

		for (I8 i = 2; i < 4; i++){
			for (I8 j = 2; j < 4; j++){

				GameObject* n = new GameObject();
				n->model.set_mesh_texture_normal(cube_mesh, texture_wood, normal_map_wood);
				n->scale = glm::vec3(10, 150, 10);
				n->model.set_tiling(1, 4);
				n->position = glm::vec3(100, 0,100) + glm::vec3(80 * i, 75, 80 * j);

				m_game_objects.push_back(n);
			}
		}

	
		CPUProfiler::end_time();
		CPUProfiler::print_time();



	}
	
	void MainGame::input(){

		if (m_input_manager.is_key_down(SDLK_d)){
			m_camera->move_right(3.0f);

		}
		if (m_input_manager.is_key_down(SDLK_a)){
			m_camera->move_left(3.0f);
		}
		if (m_input_manager.is_key_down(SDLK_w) || m_input_manager.is_key_down(SDL_BUTTON_LEFT)){
			m_camera->move_forward(3.0f);
		}
		if (m_input_manager.is_key_down(SDLK_s) || m_input_manager.is_key_down(SDL_BUTTON_RIGHT)){
			m_camera->move_back(3.0f);
		}
		if (m_input_manager.is_key_pressed(SDLK_c)){
			swap_curve = !swap_curve;
		}
	
	
		
		if (m_input_manager.is_key_down(SDLK_LEFT)){
			for (SpotLight* l : spot_lights){
				l->position.x -= 3;

			}
			for (PointLight* l : point_lights){
				l->position.x -= 3;

			}
			for (GameObject* s : spheres){
				s->position.x -= 3;
			}
		}
		if (m_input_manager.is_key_down(SDLK_RIGHT)){
			for (SpotLight* l : spot_lights){
				l->position.x += 3;

			}
			for (PointLight* l : point_lights){
				l->position.x += 3;

			}
			for (GameObject* s : spheres){
				s->position.x += 3;
			}
		}
		if (m_input_manager.is_key_down(SDLK_b)){
			static float a = 0;
			static float b = 0;
			a += 0.5;
			b += 0.5;
			
			l->set_direction(-45,b);
			p->set_direction(b,b);
		}


		if (m_input_manager.is_key_down(SDLK_n)){
			for (SpotLight* l : spot_lights){
				l->position.z -= 3;

			}
			for (PointLight* l : point_lights){
				l->position.z -= 3;

			}

			for (GameObject* s : spheres){
				s->position.z -= 3;
			}
		}
		if (m_input_manager.is_key_down(SDLK_m)){
			for (SpotLight* l : spot_lights){
				l->position.z += 3;

			}
			for (PointLight* l : point_lights){
				l->position.z += 3;

			}
			for (GameObject* s : spheres){
				s->position.z += 3;
			}
		}
		if (m_input_manager.is_key_down(SDLK_UP)){
			for (SpotLight* l : spot_lights){
				l->position.y += 3;

			}
			for (PointLight* l : point_lights){
				l->position.y += 3;

			}
			for (GameObject* s : spheres){
				s->position.y += 3;
			}
		}
		if (m_input_manager.is_key_down(SDLK_DOWN)){
			for (SpotLight* l : spot_lights){
				l->position.y -= 3;

			}
			for (PointLight* l : point_lights){
				l->position.y -= 3;

			}

			for (GameObject* s : spheres){
				s->position.y -= 3;
			}
		}
		if (m_input_manager.is_key_down(SDLK_o)){
			for (SpotLight* l : spot_lights){
				
				l->diffuse_intensity += 1;
				
			}
			for (PointLight* l : point_lights){
				l->diffuse_intensity += 1;

			}
		}
		if (m_input_manager.is_key_down(SDLK_p)){
			for (SpotLight* l : spot_lights){
				if (l->diffuse_intensity > 0)
				l->diffuse_intensity -= 1;

			}
			for (PointLight* l : point_lights){
				if (l->diffuse_intensity > 0)
					l->diffuse_intensity -= 1;

			}
		}
		if (m_input_manager.is_key_down(SDLK_k)){
			for (SpotLight* l : spot_lights){
				l->specular_intensity += 3;

			}
			for (PointLight* l : point_lights){
				l->specular_intensity += 3;

			}
		}
		if (m_input_manager.is_key_down(SDLK_l)){
			for (SpotLight* l : spot_lights){
				if (l->specular_intensity > 0)
				l->specular_intensity -= 3;

			}
			for (PointLight* l : point_lights){
				if (l->specular_intensity > 0)
					l->specular_intensity -= 3;

			}
		}

		if (m_input_manager.is_key_down(SDLK_1)){
			for (SpotLight* l : spot_lights){
				l->cutoff -= 1;
				if (l->cutoff < 5)
					l->cutoff = 5;
			}
		}

		if (m_input_manager.is_key_down(SDLK_2)){
			for (SpotLight* l : spot_lights){
				l->cutoff += 1;
				if (l->cutoff > 60)
					l->cutoff = 60;
			}
		}


		glm::vec2 mouse_coords = m_input_manager.get_mouse_movement();
		m_camera->rotate(.005f* mouse_coords.x, .005f* mouse_coords.y);

	}

	void MainGame::update(){
		time += speed;
		I32 i = 0;
		

		for (GameObject* o : m_game_objects){
		
			o->generate_model_matrix();

		}
		for (GameObject* s : spheres){

			s->generate_model_matrix();
		}



		//generate camera matrix
		m_camera->generate_VP_matrix();

		


	}

	void MainGame::draw(){

		m_deferred_renderer->init_frame();

		//geometry batch
		m_geometry_batch3d->begin();
		for (GameObject* o : m_game_objects)
			m_geometry_batch3d->draw(&(o->model));
		for (GameObject* o : spheres)
			m_geometry_batch3d->draw(&(o->model));
		m_geometry_batch3d->end();

	
		m_deferred_renderer->geometry_pass(m_geometry_batch3d);


		//shadow batch
		m_shadow_batch3d->begin();
		for (GameObject* o : m_game_objects)
			m_shadow_batch3d->draw(&(o->model));
		for (GameObject* o : spheres)
			m_shadow_batch3d->draw(&(o->model));
		m_shadow_batch3d->end();



		m_deferred_renderer->point_light_pass(&point_lights, m_shadow_batch3d);

		m_deferred_renderer->spot_light_pass(&spot_lights, m_shadow_batch3d);
		
		m_deferred_renderer->directional_lighting_pass(l, m_shadow_batch3d);

		m_deferred_renderer->skybox_pass();

		m_deferred_renderer->final_pass();

		
		//Finally swap windows
		SDL_GL_SwapWindow(m_window);
		
	
	

	}

	void MainGame::end(){
	
		m_deferred_renderer->destroy();
	}
}