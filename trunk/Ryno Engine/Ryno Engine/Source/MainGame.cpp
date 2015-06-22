#include "MainGame.h"


namespace Ryno{
	
	void MainGame::start(){

		U32 texture_metal = m_texture_loader.loadPNG("white");

		I32 cube_mesh = m_mesh_loader.load_mesh("cube");

		for (I32 x = -50; x < 51; x++){
			for (I32 z = -50; z <51; z++){


				GameObject* new_go = new GameObject();
				new_go->position = glm::vec3(x, 0, z);

				new_go->model.mesh = cube_mesh;

				new_go->model.texture = texture_metal;


				new_go->scale = glm::vec3(1, 1, 1);
				new_go->model.set_color(255, 255, 255, 255);
				m_game_objects.push_back(new_go);


			}
		}



		m_camera->position = glm::vec4(0, 30, -50, 1);

		m_batch3d->init(m_camera);

		m_program.create("prova");


	}
	
	void MainGame::input(){

		if (m_input_manager.is_key_down(SDLK_d)){
			m_camera->move_right(1.0f);

		}
		if (m_input_manager.is_key_down(SDLK_a)){
			m_camera->move_left(1.0f);
		}
		if (m_input_manager.is_key_down(SDLK_w) || m_input_manager.is_key_down(SDL_BUTTON_LEFT)){
			m_camera->move_forward(1.0f);
		}
		if (m_input_manager.is_key_down(SDLK_s) || m_input_manager.is_key_down(SDL_BUTTON_RIGHT)){
			m_camera->move_back(1.0f);
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


		glm::vec2 mouse_coords = m_input_manager.get_mouse_movement();
		m_camera->rotate(.005f* mouse_coords.x, .005f* mouse_coords.y);

	}

	void MainGame::update(){
		time += speed;
		I32 i = 0;
		for (GameObject* o : m_game_objects){

			I32 x = o->position.x;
			I32 y = o->position.y;
			I32 z = o->position.z;
			
			if (swap_curve){

				o->position.y = 5 * sin(time / 500.0f) * sin(x / 5.0f) + 5 * sin(time / 500.0f) * sin(z / 5.0f);
				o->model.set_color(255.0f, (o->position.y + 10.0f)*255.0f / 20.0f, 0.0f, 255.0f);
			}
			else{

				o->position.y = 5.0f*sin(sqrt(pow(x / 6.0f, 2) + pow(z / 6.0f, 2)) - (time / 500.0f));
				o->model.set_color(0, (o->position.y + 10.0f)*255.0f / 20.0f, 255 - (o->position.y + 10.0f)*255.0f / 20.0f, 255.0f);
			}
			
			o->generate_model_matrix();
		}

	}

	void MainGame::draw(){

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_batch3d->begin(); 

		
	
		
		for (GameObject* o : m_game_objects)
			m_batch3d->draw(&(o->model));
		
		m_batch3d->end();


		I32 t_locations = m_program.getUniformLocation("myTextureSampler");
		glUniform1i(t_locations, 0);


		m_batch3d->render_batch();

		SDL_GL_SwapWindow(m_window);


	}
}