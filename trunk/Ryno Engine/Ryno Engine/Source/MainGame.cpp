#include "MainGame.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include <GLM/gtx/transform.hpp>




namespace Ryno{

	void MainGame::start(){

		U32 texture_metal = m_texture_loader.loadPNG("white");
		cube_mesh = m_mesh_loader.load_mesh("cube");

		for (I32 x = -3; x < 4; x++){
			for (I32 z = -3; z < 4; z++){


				GameObject* new_go = new GameObject();
				new_go->position = glm::vec3(x, 0, z);

				new_go->model.mesh = cube_mesh;

				new_go->model.texture = texture_metal;


				new_go->scale = glm::vec3(1, 1, 1);
				new_go->model.set_color(255, 255, 255, 255);
				m_game_objects.push_back(new_go);


			}
		}
		U32 tex = m_texture_loader.loadPNG("moon");
		I32 mod = m_mesh_loader.load_mesh("sphere");
		for (I32 i = 0; i < 3; i++){
			GameObject* sphere = new GameObject;
			sphere->model.texture = tex;
			sphere->model.mesh = mod;
			
			sphere->scale = glm::vec3(100, 100, 100);
			sphere->model.set_color(255, 255, 255, 255);
			spheres.push_back(sphere);
		}
		spheres[0]->position = glm::vec3(-100, 100, 200);
		spheres[1]->position = glm::vec3(0, 100, 300);
		spheres[2]->position = glm::vec3(100, 100, 200);
		sound = m_audio_manager.load_sound("stomp.wav");
		music = m_audio_manager.load_music("cthulhu.ogg");
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();


		m_camera->position = glm::vec4(0, 30, -50, 1);

		m_simple_drawer = new SimpleDrawer();
		m_batch3d->init(m_camera);
	
		m_simple_drawer->init();
		m_program.create("prova");
		m_program_dir.create("dir_light");
		m_program_point.create("point_light");
		
		m_frame_buffer->send_uniforms(&m_program_dir);
		m_frame_buffer->send_uniforms(&m_program_point);

		
		square_model = m_mesh_loader.load_mesh("square");
		


		
		


		


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
		for (GameObject* o : spheres)
			o->generate_model_matrix();

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
			
			o->generate_model_matrix();
		}

	}

	void MainGame::draw(){
		m_batch3d->init(m_camera);
	
	
		//Use geometry program
		m_program.use();

		//enable depth only for geometry pass
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		//No need for blending, transparency not supported
		glDisable(GL_BLEND);

		//Bind our custom framebuffer and clear it
		m_frame_buffer->bind_for_writing();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw phase: Draw all the geometry into the buffers. Here we use the batch to do so
		m_batch3d->begin(); 

		for (GameObject* o : m_game_objects)
			m_batch3d->draw(&(o->model));
		for (GameObject* o : spheres)
			m_batch3d->draw(&(o->model));
		m_batch3d->end();

		I32 t_location = m_program.getUniformLocation("myTextureSampler");
		glUniform1i(t_location, 0);
	
		m_batch3d->render_batch();

		//Unuse the geometry program
		m_program.unuse();
	

		//Disable depth and enable blending
		glDepthMask(GL_FALSE);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		//Bind our buffer for reading (and the default one for writing)
		m_frame_buffer->bind_for_reading();

		//Clear it (not the depth, because it is not supported)
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Now we use the directional light program to draw a quad the size of the screen.
		//This quad will use the frame buffer to obtain data
		//We pass lights as uniforms
		m_program_dir.use();
		LightDirectional l(.3, .3,.6);
		glm::vec3 r = l.get_view_space_direction(m_camera);
		glUniform3f(m_program_dir.getUniformLocation("dir_light"),r.x,r.y,r.z);

		Model* s = new Model();
		s->mesh = square_model;
		m_simple_drawer->draw(s);
		glEnd();

		//The draw is done, unuse the program
		m_program_dir.unuse();


		//Same with point light
		m_program_point.use();
		LightPoint p(0,100,200);
		
		glm::vec3 z = p.get_view_space_position(m_camera);
		glUniform3f(m_program_point.getUniformLocation("point_light_pos"),z.x,z.y,z.z);
		
		
		Model* m = new Model();
		m->mesh = cube_mesh;
		glm::mat4 final_camera = m_camera->get_camera_matrix() * glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0,100,-200)), glm::vec3(100, 100, 100));
		glUniformMatrix4fv(m_program_point.getUniformLocation("MVP"), 1, GL_FALSE, &final_camera[0][0]);

		m_simple_drawer->draw(m);

		/*glBegin(GL_QUADS);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();*/
		m_program_point.unuse();

	
		//Finally swap windows
		SDL_GL_SwapWindow(m_window);
	
		
		
	


	}
}