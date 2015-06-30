#include "MainGame.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include <GLM/gtx/transform.hpp>




namespace Ryno{

	void MainGame::start(){

		U32 texture_metal = m_texture_loader.loadPNG("metal");
		cube_mesh = m_mesh_loader.load_mesh("cube");
		bound_sphere = m_mesh_loader.load_mesh("bound_sphere");
		for (I32 x = 0; x < 11; x++){
			for (I32 z = 0; z < 11; z++){


				GameObject* new_go = new GameObject();
				new_go->position = glm::vec3(x * 20, 20 * ((double)rand() / (RAND_MAX)), z*20);

				new_go->model.mesh = cube_mesh;

				new_go->model.texture = texture_metal;


				new_go->scale = glm::vec3(15,15,15);
				new_go->model.set_color(1, 1, 1, 1);
				m_game_objects.push_back(new_go);

				if (x % 2 == 0 && z % 2 == 0){
				
					LightPoint* l = new LightPoint(x * 15, 15 + 15 * ((double)rand() / (RAND_MAX)), z * 15);
					l->attenuation = .4;
					l->color = ColorRGBA(((double)rand() / (RAND_MAX)) * 255, ((double)rand() / (RAND_MAX)) * 255, ((double)rand() / (RAND_MAX))*255, 255);
					l->intensity = 20;
					point_lights.push_back(l);
				}


			}
		}
		U32 tex = m_texture_loader.loadPNG("moon");
		I32 mod = m_mesh_loader.load_mesh("sphere");
		
	
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
		m_null.create("null");
		
		m_frame_buffer->send_uniforms(&m_program_dir);
		m_frame_buffer->send_uniforms(&m_program_point);

		
		square_model = m_mesh_loader.load_mesh("square");
		
		sphere_box_model = new Model();
		sphere_box_model->mesh = bound_sphere;

		
		

		


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
	
		if (m_input_manager.is_key_down(SDLK_o)){
			for (LightPoint* l : point_lights){
				l->intensity += 1;
				if (l->attenuation < 0)l->attenuation = 0;
			}
		}
		if (m_input_manager.is_key_down(SDLK_p)){
			for (LightPoint* l : point_lights){
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

	}

	void MainGame::draw(){
	
		m_frame_buffer->start_frame();
		
		//GEOMETRY PASS
		m_program.use();
		m_frame_buffer->bind_for_geometry_pass();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	
		glDepthMask(GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		
		for (LightPoint* p : point_lights){

			//STENCIL PASS (FOR EACH  POINT LIGHT)
			m_null.use();
			m_frame_buffer->bind_for_stencil_pass();
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_ALWAYS, 0, 0);

			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

			F32 size = p->calculate_max_radius();
			
			glm::vec3 l_pos = glm::vec3(p->x, p->y, -p->z);
			glm::mat4 final_camera = m_camera->get_camera_matrix() * glm::scale(glm::translate(glm::mat4(1.0f), l_pos), glm::vec3(size, size, size));
			glUniformMatrix4fv(m_program_point.getUniformLocation("MVP"), 1, GL_FALSE, &final_camera[0][0]);

			m_simple_drawer->draw(sphere_box_model);

			m_null.unuse();

			//LIGHT PASS (FOR EACH  POINT LIGHT)
			m_frame_buffer->bind_for_light_pass();
			m_program_point.use();

			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			glm::vec3 z = p->get_view_space_position(m_camera);
			glm::vec3 color = glm::vec3(p->color.r, p->color.g, p->color.b);
			glUniform3f(m_program_point.getUniformLocation("position"), z.x, z.y, z.z);
			glUniform1f(m_program_point.getUniformLocation("attenuation"), p->attenuation);
			glUniform3f(m_program_point.getUniformLocation("color"), color.r, color.g, color.b);
			glUniform1f(m_program_point.getUniformLocation("intensity"), p->intensity);

			glUniformMatrix4fv(m_program_point.getUniformLocation("MVP"), 1, GL_FALSE, &final_camera[0][0]);

			m_simple_drawer->draw(sphere_box_model);
			glCullFace(GL_BACK);

			glDisable(GL_BLEND);
			m_program_point.unuse();
		}
		
		
		glDisable(GL_STENCIL_TEST);


		//DIRECTIONAL LIGHT PASS

		//Now we use the directional light program to draw a quad the size of the screen.
		//This quad will use the frame buffer to obtain data
		//We pass lights as

		//Not sure why, but i think because the light is invisible since i'm inside the polygon
		glCullFace(GL_FRONT);
		m_frame_buffer->bind_for_light_pass();
		m_program_dir.use();
		glDisable(GL_DEPTH_TEST);

		

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		LightDirectional l(.3f,.6f,.3f);
		l.intensity = .05;
		l.color = ColorRGBA(255, 255, 100, 255);
		glm::vec3 r = l.get_view_space_direction(m_camera);
		glUniform3f(m_program_dir.getUniformLocation("dir_light"),r.x,r.y,r.z);
		glUniform3f(m_program_dir.getUniformLocation("dir_color"), l.color.r, l.color.g, l.color.b);
		glUniform1f(m_program_dir.getUniformLocation("dir_intensity"), l.intensity);
		Model* s = new Model();
		s->mesh = square_model;
		m_simple_drawer->draw(s);
		
		glDisable(GL_BLEND);

		//The draw is done, unuse the program
		m_program_dir.unuse();

		
		//FINAL PASS (FOR EACH  POINT LIGHT)

		m_frame_buffer->bind_for_final_pass();
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glDisable(GL_CULL_FACE);

	
	

	
		//Finally swap windows
		SDL_GL_SwapWindow(m_window);
	
		
		
	


	}
}