#include "DeferredRenderer.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>


namespace Ryno{

	//Initialize deferred rendering
	void DeferredRenderer::init(Camera3D* camera){

		m_camera = camera;
		m_mesh_manager = MeshManager::get_instance();
		m_texture_manager = TextureManager::get_instance();
		m_simple_drawer = SimpleDrawer::get_instance();
		m_frame_buffer = new FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_null_program = new GLSLProgram();
		m_null_program->create("null");
		m_skybox_program = new GLSLProgram();
		m_skybox_program->create("skybox");
		m_shadow_program = new GLSLProgram();
		m_shadow_program->create("shadow");
		m_blit_program = new GLSLProgram();
		m_blit_program->create("blit");
		m_blit_program->use();
		glUniform1i(m_blit_program->getUniformLocation("screen_width"), WINDOW_WIDTH);
		glUniform1i(m_blit_program->getUniformLocation("screen_height"), WINDOW_HEIGHT);
		glUniform1i(m_blit_program->getUniformLocation("source_buffer"), 0);

		m_bounding_box = new Model();
		m_fullscreen_quad = new Model();
		m_cube_box = new Model();
		m_cube_box->mesh = m_mesh_manager->load_mesh("simple_cube");
		m_bounding_box->mesh = m_mesh_manager->load_mesh("bound_sphere");
		m_fullscreen_quad->mesh = m_mesh_manager->load_mesh("square");

	}

	//Call before drawing geometry
	void DeferredRenderer::init_geometric_pass(){

		
		inverse_P = glm::inverse(m_camera->get_projection_matrix());

		m_frame_buffer->start_frame();

		//GEOMETRY PASS
		m_frame_buffer->bind_for_geometry_pass();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	




	//Apply point lights
	void DeferredRenderer::point_light_pass(std::vector<PointLight*>* point_lights){
		glDepthMask(GL_FALSE);
		glEnable(GL_STENCIL_TEST);


		for (PointLight* p : *point_lights){
			stencil_pass(p);
			light_pass(p);
		}
		glDisable(GL_STENCIL_TEST);

	}

	//Stencil pass for point lights only.
	//Call for each light inside the light pass
	void DeferredRenderer::stencil_pass(PointLight* point_light){

		m_null_program->use();
		m_frame_buffer->bind_for_stencil_pass();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		F32 size = point_light->calculate_max_radius();
		glm::vec3 temp_pos = point_light->position;
		temp_pos.z *= -1;
		MVP_camera = m_camera->get_camera_matrix() * glm::scale(glm::translate(glm::mat4(1.0f),temp_pos ), glm::vec3(size, size, size));
		glUniformMatrix4fv(m_null_program->getUniformLocation("MVP"), 1, GL_FALSE, &MVP_camera[0][0]);

		m_simple_drawer->draw(m_bounding_box);

		m_null_program->unuse();
	}

	//Renders point light inside it's bounding sphere
	void DeferredRenderer::light_pass(PointLight* point_light){

		m_frame_buffer->bind_for_light_pass();
		point_light->program->use();
		glUniformMatrix4fv(point_light->program->getUniformLocation("inverse_P_matrix"), 1, GL_FALSE, &inverse_P[0][0]);

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		point_light->send_uniforms(m_camera);

		glUniformMatrix4fv(point_light->program->getUniformLocation("MVP"), 1, GL_FALSE, &MVP_camera[0][0]);
		m_simple_drawer->draw(m_bounding_box);
		glCullFace(GL_BACK);

		glDisable(GL_BLEND);
		point_light->program->unuse();
	}


	void DeferredRenderer::shadow_pass(DirectionalLight* directional_light){
		m_frame_buffer->bind_for_shadow_map_pass();
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		m_shadow_program->use();
		glm::vec3 inv_dir = directional_light->direction.to_vec3();
		inv_dir.z *= -1;
		glm::mat4 ortho_mat = m_camera->get_ortho_matrix();
		glm::mat4 view_mat = glm::lookAt(inv_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 final_mat = ortho_mat * view_mat;


		glUniformMatrix4fv(m_shadow_program->getUniformLocation("light_VP"), 1, GL_FALSE, &final_mat[0][0]);
		//m_simple_drawer->draw(m_bounding_box);
		//m_shadow_program->unuse();
		//glDepthMask(GL_FALSE);
	}


	//Apply diretional light
	void DeferredRenderer::directional_light_pass(DirectionalLight* directional_light)
	{
		m_frame_buffer->bind_for_light_pass();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		glDisable(GL_DEPTH_TEST);

		directional_light->program->use();
		static const glm::mat4 bias(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);
		glm::vec3 inv_dir = directional_light->direction.to_vec3();
		inv_dir.z *= -1;
		glm::mat4 ortho_mat = m_camera->get_ortho_matrix();
		glm::mat4 view_mat = glm::lookAt(inv_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 final_mat = bias * ortho_mat * view_mat;

		
		glUniformMatrix4fv(directional_light->program->getUniformLocation("light_VP_matrix"), 1, GL_FALSE, &final_mat[0][0]);

		glUniformMatrix4fv(directional_light->program->getUniformLocation("inverse_P_matrix"), 1, GL_FALSE, &inverse_P[0][0]);
		glm::mat4 inverse_VP_matrix = glm::inverse(m_camera->get_camera_matrix());
		glUniformMatrix4fv(directional_light->program->getUniformLocation("inverse_VP_matrix"), 1, GL_FALSE, &inverse_VP_matrix[0][0]);

		


		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		directional_light->send_uniforms(m_camera);
		m_simple_drawer->draw(m_fullscreen_quad);

		glDisable(GL_BLEND);
		

		//The draw is done, unuse the program
		directional_light->program->unuse();
	}

	


	void DeferredRenderer::skybox_pass(){
		m_frame_buffer->bind_for_skybox_pass();
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);

		//copy depth buffer (the one created by geometry pass) inside the actual depth buffer to test
		m_blit_program->use();
		m_simple_drawer->draw(m_fullscreen_quad);
		m_blit_program->unuse();

		glDepthMask(GL_FALSE);
		

		//To draw sky at infinite Z
		glDepthRange(0.99999, 1.0);

		
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		m_skybox_program->use();

		//Remove translation from VP matrix
		glm::mat4 no_trans_VP = m_camera->get_projection_matrix() *  glm::mat4(glm::mat3(m_camera->get_view_matrix()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_camera->skybox);
		glUniformMatrix4fv(m_skybox_program->getUniformLocation("no_trans_VP"), 1, GL_FALSE, &no_trans_VP[0][0]);
		glUniform1i(m_skybox_program->getUniformLocation("cube_map"), 0);


		m_simple_drawer->draw(m_cube_box);

		m_skybox_program->unuse();

		//Restore depth
		glDepthRange(0.0, 1.0);
		glDepthMask(GL_TRUE);



	}


	//Print on screen the result of the whole deferred rendering
	void DeferredRenderer::final_pass(){
		m_frame_buffer->bind_for_final_pass();

		/*glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/
	}

	


	void DeferredRenderer::destroy(){
		free(m_frame_buffer);
		free(m_bounding_box);
		free(m_fullscreen_quad);
		m_null_program->destroy();
		m_blit_program->destroy();
		m_shadow_program->destroy();
	}


	
}