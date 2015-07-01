#include "MainGame.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <GLM/gtx/transform.hpp>




namespace Ryno{

	void MainGame::start(){

		m_simple_drawer = SimpleDrawer::get_instance();

		U32 texture_metal = m_texture_loader->loadPNG("metal");
		cube_mesh = m_mesh_manager->load_mesh("cube");
		bound_sphere = m_mesh_manager->load_mesh("bound_sphere");
		for (I32 x = -11; x < 12; x++){
			for (I32 z = -11; z < 12; z++){


				GameObject* new_go = new GameObject();
				new_go->position = glm::vec3(x * 10, 4 *z, z*10);

				new_go->model.mesh = cube_mesh;

				new_go->model.texture = texture_metal;


				new_go->scale = glm::vec3(10,10,10);
				new_go->model.set_color(1, 1, 1, 1);
				m_game_objects.push_back(new_go);

				if (x % 2 == 0 && z % 2 == 0){
				
					PointLight* l = new PointLight(x * 10, 5.9+ 4 * z, z * 10);
					l->attenuation = .9;
					l->color = ColorRGBA(((double)rand() / (RAND_MAX)) * 255, ((double)rand() / (RAND_MAX)) * 255, ((double)rand() / (RAND_MAX))*255, 255);
					l->intensity = 1;
					l->program = &m_program_point;
					point_lights.push_back(l);
				}
				else{
					
					 new_go->position.y += 4;
					
				}


			}
		}
		U32 tex = m_texture_loader->loadPNG("moon");
		I32 mod = m_mesh_manager->load_mesh("sphere");
		
	
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
		glUniform1i(m_program_dir.getUniformLocation("x"), WINDOW_WIDTH);
		glUniform1i(m_program_dir.getUniformLocation("y"), WINDOW_HEIGHT);
		glUniform1i(m_program_dir.getUniformLocation("m_col"), 1);
		glUniform1i(m_program_dir.getUniformLocation("m_nor"), 2);

		m_program_point.use();
		glUniform1i(m_program_point.getUniformLocation("x"), WINDOW_WIDTH);
		glUniform1i(m_program_point.getUniformLocation("y"), WINDOW_HEIGHT);
		glUniform1i(m_program_point.getUniformLocation("m_pos"), 0);
		glUniform1i(m_program_point.getUniformLocation("m_col"), 1);
		glUniform1i(m_program_point.getUniformLocation("m_nor"), 2);
		m_program_point.unuse();

		
		square_model = m_mesh_manager->load_mesh("square");
		
		sphere_box_model = new Model();
		sphere_box_model->mesh = bound_sphere;
		l = new DirectionalLight(.3f, .6f, .3f);
		l->intensity = .15;
		l->color = ColorRGBA(255, 255, 255, 255);
		l->program = &m_program_dir;


		m_deferred_renderer = new DeferredRenderer();
		m_deferred_renderer->init(m_camera);
		
		

		


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
			speed--;
		}
		if (m_input_manager.is_key_down(SDLK_RIGHT)){
			speed++;
		}
	
		if (m_input_manager.is_key_down(SDLK_o)){
			for (PointLight* l : point_lights){
				l->intensity += 1;
				if (l->attenuation < 0)l->attenuation = 0;
			}
		}
		if (m_input_manager.is_key_down(SDLK_p)){
			for (PointLight* l : point_lights){
				l->intensity -= 1;
				
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

		for (PointLight* l : point_lights){
			//l->y += sin(time/500)/5;
			//l->z -= cos(time / 500) / 5;
			//l->x += cos(time / 500) / 5;
			
		}

	}

	void MainGame::draw(){
	
		

		m_deferred_renderer->init_geometric_pass();

		m_program.use();

		m_batch3d->begin(); 

		for (GameObject* o : m_game_objects)
			m_batch3d->draw(&(o->model));
		for (GameObject* o : spheres)
			m_batch3d->draw(&(o->model));
		m_batch3d->end();

		I32 t_location = m_program.getUniformLocation("myTextureSampler");
		glUniform1i(t_location, 0);
	
		m_batch3d->render_batch();

		m_program.unuse();
		
		m_deferred_renderer->point_light_pass(&point_lights);
		m_deferred_renderer->directional_light_pass(l);
		m_deferred_renderer->final_pass();
		
		//Finally swap windows
		SDL_GL_SwapWindow(m_window);
	

	}
}