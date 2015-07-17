#include "MainGame.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "GPUProfiler.h"
#include <GLM/gtx/transform.hpp>





namespace Ryno{

	void MainGame::start(){

		m_simple_drawer = SimpleDrawer::get_instance();

		U32 texture_metal = m_texture_loader->loadPNG("metal");
		U32 white = m_texture_loader->loadPNG("white_pixel");
		U32 white_normal = m_texture_loader->loadPNG("normal_pixel");


		U32 tex = m_texture_loader->loadPNG("moon");
		U32 texture_bricks = m_texture_loader->loadPNG("pack/177");
		U32 normal_map_bricks = m_texture_loader->loadPNG("pack/177_norm");
		U32 texture_red_wall = m_texture_loader->loadPNG("pack/179");
		U32 normal_map_red_wall = m_texture_loader->loadPNG("pack/179_norm");
		U32 texture_grey_wall = m_texture_loader->loadPNG("pack/178");
		U32 normal_map_grey_wall = m_texture_loader->loadPNG("pack/178_norm");
		U32 texture_oblique = m_texture_loader->loadPNG("pack/199");
		U32 normal_map_oblique = m_texture_loader->loadPNG("pack/199_norm");

		cube_mesh = m_mesh_manager->load_mesh("cube");
		bound_sphere = m_mesh_manager->load_mesh("bound_sphere");
		I32 sphere_model = m_mesh_manager->load_mesh("sphere");
		
	
		sound = m_audio_manager.load_sound("stomp.wav");
		music = m_audio_manager.load_music("cthulhu.ogg");
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();


		m_camera->position = glm::vec4(0, 30, -50, 1);

		m_batch3d->init(m_camera);
		

		m_program_geometry.create("geometry");

		m_program_dir.create("dir_light");
		m_program_point.create("point_light");
		m_program_flat.create("flat");
		
		m_program_geometry.use();
		glUniform1i(m_program_geometry.getUniformLocation("texture_sampler"), 0);
		glUniform1i(m_program_geometry.getUniformLocation("normal_map_sampler"), 1);
		m_program_geometry.unuse();

		m_program_dir.use();
		glUniform1i(m_program_dir.getUniformLocation("screen_width"), WINDOW_WIDTH);
		glUniform1i(m_program_dir.getUniformLocation("screen_height"), WINDOW_HEIGHT);
		glUniform1i(m_program_dir.getUniformLocation("g_color_tex"), 0);
		glUniform1i(m_program_dir.getUniformLocation("g_normal_tex"), 1);
		glUniform1i(m_program_dir.getUniformLocation("g_depth_tex"), 2);
		

		m_program_point.use();
		glUniform1i(m_program_point.getUniformLocation("screen_width"), WINDOW_WIDTH);
		glUniform1i(m_program_point.getUniformLocation("screen_height"), WINDOW_HEIGHT);
		glUniform1i(m_program_point.getUniformLocation("g_color_tex"), 0);
		glUniform1i(m_program_point.getUniformLocation("g_normal_tex"), 1);
		glUniform1i(m_program_point.getUniformLocation("g_depth_tex"), 2);


		m_program_point.unuse();

		
		square_model = m_mesh_manager->load_mesh("square");
		
		sphere_box_model = new Model();
		sphere_box_model->mesh = bound_sphere;
		

		m_deferred_renderer = new DeferredRenderer();
		m_deferred_renderer->init(m_camera);
		
		
	

		
		////Simple room
		GameObject* base = new GameObject();
		base->model.set_color_and_flatness(255, 255, 255, 0);
		base->model.set_mesh_texture_normal(cube_mesh, texture_bricks, normal_map_bricks);
		base->scale = glm::vec3(2000, 10, 2000);
		base->model.set_tiling(50, 50);
		base->position = glm::vec3(0, 0, 0);
		m_game_objects.push_back(base);

		GameObject* back = new GameObject();
		back->model.set_mesh_texture_normal(cube_mesh, texture_red_wall, normal_map_red_wall);
		back->scale = glm::vec3(220, 400, 10);
		back->model.set_tiling(5, 10);
		back->position = glm::vec3(0, 100, 200);

		m_game_objects.push_back(back);

		GameObject* left = new GameObject();
		left->model.set_mesh_texture_normal(cube_mesh, texture_red_wall, normal_map_red_wall);
		left->scale = glm::vec3(10, 400, 400);
		left->position = glm::vec3(-110, 100, 0);
		left->model.set_tiling(10,10);
		m_game_objects.push_back(left);

		GameObject* right = new GameObject();
		right->model.set_mesh_texture_normal(cube_mesh, texture_red_wall, normal_map_red_wall);
		right->scale = glm::vec3(10, 400, 400);
		right->model.set_tiling(10,10);
		right->position = glm::vec3(110, 100, 0);

		m_game_objects.push_back(right);

	
		
		GameObject* bl;
		PointLight* p;

		p = new PointLight(0,120,120);
		
		p->set_diffuse_color(255,0,0);
		p->diffuse_intensity = 700;
		p->attenuation = .1;
		p->specular_intensity = 700;
		p->set_specular_color(255,0,0);
		p->program = &m_program_point;
		point_lights.push_back(p);
		bl = new GameObject();
		bl->model.set_color_and_flatness(0,0,0, 255);
		bl->model.set_mesh_texture_normal(sphere_model, white, white_normal);
		bl->scale = glm::vec3(100,100,30);
		bl->position = glm::vec3(0,120,120);
		spheres.push_back(bl);

		for (int i = -3; i < 4; i++){
			F32 n = 36.5;
			F32 k = i + 3;

			U8 r =n*k;
			U8 g = n*k;
			U8 b = 255.0 - (n*k);
			for (int j = -3; j < 4; j++){
				
				p = new PointLight(30 * j, 10, 30 * i );
				
				p->set_diffuse_color(r,g,b);
				p->diffuse_intensity = 10;
				p->attenuation = .1;
				p->specular_intensity = 50;
				p->set_specular_color(r,g,b);
				p->program = &m_program_point;
				point_lights.push_back(p);
				bl = new GameObject();
				bl->model.set_color_and_flatness(220 + 35 * r / 255, 220 + 35 * g / 255, 220 + 35 * b / 255, 255);
				bl->model.set_mesh_texture_normal(sphere_model, white, white_normal);
				bl->scale = glm::vec3(10,10,10);
				bl->position = glm::vec3(30 * j, 10, 30 * i);
				spheres.push_back(bl);
			}
		}
		
		l = new DirectionalLight(0,0,1);
		l->diffuse_intensity = 0.05;
		l->set_diffuse_color(255,0,0);
		l->specular_intensity = 0;
		l->set_specular_color(255, 255, 255);
		l->ambient_intensity = 0.05;
		l->set_ambient_color(255, 255, 0);
		l->program = &m_program_dir;


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
			for (PointLight* l : point_lights){
				l->position.x -= 1;

			}
			for (GameObject* s : spheres){
				s->position.x -= 1;
			}
		}
		if (m_input_manager.is_key_down(SDLK_RIGHT)){
			for (PointLight* l : point_lights){
				l->position.x += 1;

			}
			for (GameObject* s : spheres){
				s->position.x += 1;
			}
		}

		if (m_input_manager.is_key_down(SDLK_n)){
			for (PointLight* l : point_lights){
				l->position.z -= 1;

			}
			for (GameObject* s : spheres){
				s->position.z -= 1;
			}
		}
		if (m_input_manager.is_key_down(SDLK_m)){
			for (PointLight* l : point_lights){
				l->position.z += 1;

			}
			for (GameObject* s : spheres){
				s->position.z+=1;
			}
		}
		if (m_input_manager.is_key_down(SDLK_UP)){
			for (PointLight* l : point_lights){
				l->position.y += 1;

			}
			for (GameObject* s : spheres){
				s->position.y += 1;
			}
		}
		if (m_input_manager.is_key_down(SDLK_DOWN)){
			for (PointLight* l : point_lights){
				l->position.y -= 1;

			}
			for (GameObject* s : spheres){
				s->position.y -= 1;
			}
		}
		if (m_input_manager.is_key_down(SDLK_o)){
			for (PointLight* l : point_lights){
				
				l->diffuse_intensity += 1;
				
			}
		}
		if (m_input_manager.is_key_down(SDLK_p)){
			for (PointLight* l : point_lights){
				if (l->diffuse_intensity > 0)
				l->diffuse_intensity -= 1;

			}
		}
		if (m_input_manager.is_key_down(SDLK_k)){
			for (PointLight* l : point_lights){
				l->specular_intensity += 3;

			}
		}
		if (m_input_manager.is_key_down(SDLK_l)){
			for (PointLight* l : point_lights){
				if (l->specular_intensity > 0)
				l->specular_intensity -= 3;

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

		


	}

	void MainGame::draw(){
	


		GPUProfiler::begin(3);
			
			

		m_deferred_renderer->init_geometric_pass();

		m_program_geometry.use();

		

		m_batch3d->begin(); 
		for (GameObject* o : m_game_objects)
			m_batch3d->draw(&(o->model));
		for (GameObject* o : spheres)
			m_batch3d->draw(&(o->model));
		
		m_batch3d->end();

		GPUProfiler::start_time();
		m_batch3d->render_batch();
		GPUProfiler::end_time();

		m_program_geometry.unuse();

		


		GPUProfiler::start_time();
		m_deferred_renderer->point_light_pass(&point_lights);
		GPUProfiler::end_time();

		GPUProfiler::start_time();
		m_deferred_renderer->directional_light_pass(l);
		GPUProfiler::end_time();

		
		m_deferred_renderer->final_pass();
		
		//
		//Finally swap windows
		SDL_GL_SwapWindow(m_window);
		
	
		//GPUProfiler::print_time();
	

	}

	void MainGame::end(){
		m_program_geometry.destroy();
		m_program_dir.destroy();
		m_program_point.destroy();
		m_program_flat.destroy();
		m_deferred_renderer->destroy();
	}
}