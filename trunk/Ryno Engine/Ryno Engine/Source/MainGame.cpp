#include "MainGame.h"


namespace Ryno{
	
	void MainGame::start(){

		U32 texture_metal = m_texture_loader.loadPNG("white");
		I32 cube_mesh = m_mesh_loader.load_mesh("cube");

		for (I32 x = -3; x < 4; x++){
			for (I32 z = -3; z <4; z++){


				GameObject* new_go = new GameObject();
				new_go->position = glm::vec3(x,0,z);

				new_go->model.mesh = cube_mesh;

				new_go->model.texture = texture_metal;


				new_go->scale = glm::vec3(1, 1, 1);
				new_go->model.set_color(255, 255, 255, 255);
				m_game_objects.push_back(new_go);


			}
		}
		
		sphere = new GameObject();
		sphere->model.texture = m_texture_loader.loadPNG("moon");
		sphere->model.mesh = m_mesh_loader.load_mesh("sphere");
		sphere->position = glm::vec3(0, 100, 240);
		sphere->scale = glm::vec3(100, 100, 100);
		sphere->model.set_color(255, 255, 255, 255);
		sound = m_audio_manager.load_sound("stomp.wav");
		music = m_audio_manager.load_music("cthulhu.ogg");
		sound.set_volume(1.0f);
		music.set_volume(0.5f);
		music.play();


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
			
			if (!just_played && sin(time / 500.0f) <-.70 && sin(time / 500.0f) >-.80){
				sound.play();
				just_played = true;
			}
			else if (just_played && sin(time / 500.0f) <-.99)
				just_played = false;


			if (swap_curve){

				//o->position.y = 5 * (sin(time / 500.0f) * sin(x / 5.0f) +  sin(time / 500.0f) * sin(z / 5.0f));
				//o->model.set_color(255.0f, (o->position.y + 10.0f)*255.0f / 20.0f, 0.0f, 255.0f);
			}
			else{
			
				o->position.y = 5.0f*sin(sqrt(pow(x / 6.0f, 2) + pow(z / 6.0f, 2)) - (time / 500.0f));
				o->model.set_color(0, (o->position.y + 10.0f)*255.0f / 20.0f, 255 - (o->position.y + 10.0f)*255.0f / 20.0f, 255.0f);
			}
			sphere->generate_model_matrix();
			o->generate_model_matrix();
		}

	}

	void MainGame::draw(){
		std::cout << "yolo" << std::endl;

		m_frame_buffer->bind_for_writing();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_batch3d->begin(); 

		
	
		
		for (GameObject* o : m_game_objects)
			m_batch3d->draw(&(o->model));
		m_batch3d->draw(&(sphere->model));
		m_batch3d->end();


		I32 t_location = m_program.getUniformLocation("myTextureSampler");
		glUniform1i(t_location, 0);
		/*I32 l_location = m_program.getUniformLocation("light_direction");
		glm::vec4 res = glm::normalize(m_camera->get_view_matrix()* glm::vec4(1,.3,0, 0));
		glUniform3f(l_location, res.x,res.y,res.z);*/


		m_batch3d->render_batch();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_frame_buffer->bind_for_reading();

		U32 HalfWidth = WINDOW_WIDTH / 2.0f;
		U32 HalfHeight = WINDOW_HEIGHT / 2.0f;

		m_frame_buffer->set_read_buffer(FrameBuffer::FRAME_TEXTURE_TYPE_POSITION);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		m_frame_buffer->set_read_buffer(FrameBuffer::FRAME_TEXTURE_TYPE_DIFFUSE);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, HalfHeight, HalfWidth, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		m_frame_buffer->set_read_buffer(FrameBuffer::FRAME_TEXTURE_TYPE_NORMAL);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		m_frame_buffer->set_read_buffer(FrameBuffer::FRAME_TEXTURE_TYPE_TEXCOORD);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		SDL_GL_SwapWindow(m_window);


	}
}