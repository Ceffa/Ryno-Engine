#include "MainGame.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <GLM/gtx/transform.hpp>




namespace Ryno{

	void MainGame::start(){

		m_simple_drawer = SimpleDrawer::get_instance();

		U32 texture_metal = m_texture_loader->loadPNG("metal");
		U32 white = m_texture_loader->loadPNG("white");
		cube_mesh = m_mesh_manager->load_mesh("cube");
		bound_sphere = m_mesh_manager->load_mesh("bound_sphere");
		
		

		U32 tex = m_texture_loader->loadPNG("moon");
		I32 sphere_model = m_mesh_manager->load_mesh("sphere");
		
	
		sound = m_audio_manager.load_sound("stomp.wav");
		music = m_audio_manager.load_music("cthulhu.ogg");
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();


		m_camera->position = glm::vec4(0, 30, -50, 1);

		m_batch3d->init(m_camera);
	
		m_program.create("prova");
		m_program_dir.create("dir_light");
		m_program_point.create("point_light");
		m_null.create("null");
		
	
		m_program_dir.use();
		glUniform1i(m_program_dir.getUniformLocation("screen_width"), WINDOW_WIDTH);
		glUniform1i(m_program_dir.getUniformLocation("screen_height"), WINDOW_HEIGHT);
		glUniform1i(m_program_point.getUniformLocation("g_position_tex"), 0);
		glUniform1i(m_program_dir.getUniformLocation("g_color_tex"), 1);
		glUniform1i(m_program_dir.getUniformLocation("g_normal_tex"), 2);

		m_program_point.use();
		glUniform1i(m_program_point.getUniformLocation("screen_width"), WINDOW_WIDTH);
		glUniform1i(m_program_point.getUniformLocation("screen_height"), WINDOW_HEIGHT);
		glUniform1i(m_program_point.getUniformLocation("g_position_tex"), 0);
		glUniform1i(m_program_point.getUniformLocation("g_color_tex"), 1);
		glUniform1i(m_program_point.getUniformLocation("g_normal_tex"), 2);
		m_program_point.unuse();

		
		square_model = m_mesh_manager->load_mesh("square");
		
		sphere_box_model = new Model();
		sphere_box_model->mesh = bound_sphere;
		

		m_deferred_renderer = new DeferredRenderer();
		m_deferred_renderer->init(m_camera);
		
		U32 texture_brick = m_texture_loader->loadPNG("brick");
		U32 normal_map_brick = m_texture_loader->loadPNG("brick_normal");
		U32 pixel_normal = m_texture_loader->loadPNG("pixel_normal");
		Model* m = new Model();
		m->mesh = cube_mesh;
		m->texture = texture_brick;
		m->normal_map = normal_map_brick;

		
		//Simple room
		GameObject* base = new GameObject();
		base->model = *m;
		base->scale = glm::vec3(200, 10, 200);
		base->position = glm::vec3(0, 0, -50);
		m_game_objects.push_back(base);

		GameObject* back = new GameObject();
		back->model = *m;
		back->scale = glm::vec3(210, 100, 10);
		back->position = glm::vec3(0, 45, 55);

		m_game_objects.push_back(back);

		GameObject* left = new GameObject();
		left->model = *m;
		left->scale = glm::vec3(10, 100, 100);
		left->position = glm::vec3(-100, 45, 0);
		m_game_objects.push_back(left);

		GameObject* right = new GameObject();
		right->model = *m;
		right->scale = glm::vec3(10, 100, 100);
		right->position = glm::vec3(100, 45, 0);

		m_game_objects.push_back(right);

		ball = new GameObject();
		ball->model.set_color(255, 170, 0, 255);
		ball->model.mesh = sphere_model;
		ball->model.texture = white;
		ball->model.normal_map = pixel_normal;
		ball->scale = glm::vec3(5, 5, 5);
		ball->position = glm::vec3(0, 50, 20);
		m_game_objects.push_back(ball);

		PointLight* p = new PointLight(0, 50,20);
		p->set_diffuse_color(255, 170, 0, 255);
		p->diffuse_intensity = 10;
		p->attenuation = .1; 
		p->specular_intensity = 50;
		p->set_specular_color(255,170,0,255);
		p->program = &m_program_point;
		point_lights.push_back(p);
		
		l = new DirectionalLight(.3f, .3f, .3f);
		l->diffuse_intensity = .3;
		l->set_diffuse_color(255, 255, 255, 255);
		l->specular_intensity = .5;
		l->set_specular_color(255, 255, 255, 255);
		l->ambient_intensity = .1;
		l->set_ambient_color(255, 255, 255, 255);
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
			ball->position.x -= 1;
		}
		if (m_input_manager.is_key_down(SDLK_RIGHT)){
			for (PointLight* l : point_lights){
				l->position.x += 1;

			}
			ball->position.x += 1;
		}
		if (m_input_manager.is_key_down(SDLK_UP)){
			for (PointLight* l : point_lights){
				l->position.y += 1;

			}
			ball->position.y += 1;
		}
		if (m_input_manager.is_key_down(SDLK_DOWN)){
			for (PointLight* l : point_lights){
				l->position.y -= 1;

			}
			ball->position.y -= 1;
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
				l->specular_intensity += 1;

			}
		}
		if (m_input_manager.is_key_down(SDLK_l)){
			for (PointLight* l : point_lights){
				if (l->specular_intensity > 0)
				l->specular_intensity -= 1;

			}
		}


		glm::vec2 mouse_coords = m_input_manager.get_mouse_movement();
		m_camera->rotate(.005f* mouse_coords.x, .005f* mouse_coords.y);

	}

	void MainGame::update(){
		time += speed;
		I32 i = 0;
		for (GameObject* o : spheres)
			o->generate_model_matrix();

		for (GameObject* o : m_game_objects){

			//I32 x = o->position.x;
			//I32 y = o->position.y;
			//I32 z = o->position.z;
			//
			//if (!just_played && sin(time / 500.0f) <-.70 && sin(time / 500.0f) >-.80){
			//	sound.play();
			//	just_played = true;
			//}
			//else if (just_played && sin(time / 500.0f) <-.99)
			//	just_played = false;


			//if (swap_curve){

			//	//o->position.y = 5 * (sin(time / 500.0f) * sin(x / 5.0f) +  sin(time / 500.0f) * sin(z / 5.0f));
			//	//o->model.set_color(255.0f, (o->position.y + 10.0f)*255.0f / 20.0f, 0.0f, 255.0f);
			//}
			//else{
			//
			//	o->position.y = 5.0f*sin(sqrt(pow(x / 6.0f, 2) + pow(z / 6.0f, 2)) - (time / 500.0f));
			//	//o->model.set_color(0, (o->position.y + 10.0f)*255.0f / 20.0f, 255 - (o->position.y + 10.0f)*255.0f / 20.0f, 255.0f);
			//}
			
			o->generate_model_matrix();
		}
		


	}

	void MainGame::draw(){
	
		

		m_deferred_renderer->init_geometric_pass();

		m_program.use();

		I32 t_location = m_program.getUniformLocation("texture_sampler");
		glUniform1i(t_location, 0);
		I32 n_m_location = m_program.getUniformLocation("normal_map_sampler");
		glUniform1i(n_m_location, 1);

		m_batch3d->begin(); 

		for (GameObject* o : m_game_objects)
			m_batch3d->draw(&(o->model));
		for (GameObject* o : spheres)
			m_batch3d->draw(&(o->model));
		m_batch3d->end();


		
		
	
		m_batch3d->render_batch();

		m_program.unuse();
		
		//m_deferred_renderer->debug_geometry_pass();
		m_deferred_renderer->point_light_pass(&point_lights);
		m_deferred_renderer->directional_light_pass(l);
		m_deferred_renderer->final_pass();
		
		//Finally swap windows
		SDL_GL_SwapWindow(m_window);
	

	}
}