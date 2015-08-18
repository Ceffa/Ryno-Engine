#include "DeferredRenderer.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#define PI 3.14159265359


namespace Ryno{

	const CameraDirection DeferredRenderer::camera_directions[NUM_OF_LAYERS]=
	{
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }
	};

	//Initialize deferred rendering
	void DeferredRenderer::init(Camera3D* camera){

		m_camera = camera;
		m_mesh_manager = MeshManager::get_instance();
		m_texture_manager = TextureManager::get_instance();
		m_simple_drawer = SimpleDrawer::get_instance();
		m_fbo_deferred = new FBO_Deferred(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_fbo_shadow = new FBO_Shadow(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_null_program = new GLSLProgram();
		m_null_program->create("null");
		m_skybox_program = new GLSLProgram();
		m_skybox_program->create("skybox");
		m_directional_shadow_program = new GLSLProgram();
		m_directional_shadow_program->create("directional_shadow");
		m_point_shadow_program = new GLSLProgram();
		m_point_shadow_program->create("point_shadow");

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

	void DeferredRenderer::init_frame(){

		//Calculate camera matrix once and for all
		inverse_P = glm::inverse(m_camera->get_projection_matrix());

		//Setup the two fbos for this frame
		m_fbo_deferred->start_frame();
		m_fbo_shadow->start_frame();

	}

	//Call before drawing geometry
	void DeferredRenderer::geometry_pass(Batch3DGeometry* batch, GLSLProgram* program){

		m_fbo_deferred->bind_for_geometry_pass();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		program->use();
		//Setup geometry program
		glUniformMatrix4fv(program->getUniformLocation("V"), 1, GL_FALSE, &m_camera->get_view_matrix()[0][0]);
		glUniformMatrix4fv(program->getUniformLocation("VP"), 1, GL_FALSE, &m_camera->get_camera_matrix()[0][0]);
		batch->render_batch();
		program->unuse();
	}

	void DeferredRenderer::point_shadow_pass(std::vector<PointLight*>* point_lights, Batch3DShadow* batch)
	{
		//Enable depth testing and writing
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_CULL_FACE);
		
		//Set viewportto cubemap size (because the next rendering will not be at fullscreen)
		glViewport(0, 0, m_fbo_shadow->cube_shadow_resolution, m_fbo_shadow->cube_shadow_resolution);


		PointLight* p = point_lights->back();

		//Set default color to the maximum
		glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

		
		//For each face
		for (U8 i = 0; i < NUM_OF_LAYERS; i++){

			//Bind the correct cube face, and clear it
			m_fbo_shadow->bind_face(camera_directions[i].CubemapFace);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Get light position, with correct z axis
			glm::vec3 correct_position = glm::vec3(p->position.x, p->position.y, -p->position.z);

			//Get view matrix of the light. This is both translate and rotate matrix
			glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + camera_directions[i].Target, camera_directions[i].Up);
			
			//Multiply view by the standard projection matrix for cubemaps (precalculated)
			glm::mat4 light_VP_matrix = m_fbo_shadow->point_shadow_projection_matrix * view_matrix;// proj_matrix * view_matrix;
			
			//Send Vp matrix and world light position to shader, then render
			m_point_shadow_program->use();
			glUniformMatrix4fv(m_point_shadow_program->getUniformLocation("light_VP"), 1, GL_FALSE, &light_VP_matrix[0][0]);
			glUniform3f(m_point_shadow_program->getUniformLocation("light_world_pos"), p->position.x, p->position.y,p->position.z);
			batch->render_batch();
			m_point_shadow_program->unuse();

			//Blit for debugging purposes, to be disabled
			m_fbo_shadow->blit_to_debug(i);
		}

		//Restore clear color and viewport
		glClearColor(0, 0, 0, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


	}

	void DeferredRenderer::directional_shadow_pass(DirectionalLight* directional_light, Batch3DShadow* batch){

		m_fbo_shadow->bind_for_shadow_map_pass();
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		//generate light_VP matrix
		glm::vec3 inv_dir = directional_light->direction.to_vec3();
		inv_dir.z *= -1;
		glm::mat4 ortho_mat = m_camera->get_ortho_matrix();
		glm::mat4 view_mat = glm::lookAt(inv_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 final_mat = ortho_mat * view_mat;

		m_directional_shadow_program->use();
		glUniformMatrix4fv(m_directional_shadow_program->getUniformLocation("light_VP"), 1, GL_FALSE, &final_mat[0][0]);
		batch->render_batch();
		m_directional_shadow_program->unuse();



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

		
		m_fbo_deferred->bind_for_stencil_pass();

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
		
		m_null_program->use();
		glUniformMatrix4fv(m_null_program->getUniformLocation("MVP"), 1, GL_FALSE, &MVP_camera[0][0]);
		m_simple_drawer->draw(m_bounding_box);
		m_null_program->unuse();
	}


	

	//Renders point lights inside it's bounding sphere
	void DeferredRenderer::light_pass(PointLight* point_light){

		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_point_light_pass();

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		point_light->program->use();
		point_light->send_uniforms(m_camera);
		glUniformMatrix4fv(point_light->program->getUniformLocation("inverse_P_matrix"), 1, GL_FALSE, &inverse_P[0][0]);
		glUniformMatrix4fv(point_light->program->getUniformLocation("MVP"), 1, GL_FALSE, &MVP_camera[0][0]);
		m_simple_drawer->draw(m_bounding_box);
		point_light->program->unuse();

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	}


	


	//Apply diretional light
	void DeferredRenderer::directional_light_pass(DirectionalLight* directional_light)
	{
		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_directional_light_pass();

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
		m_fbo_deferred->bind_for_skybox_pass();
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
		m_fbo_deferred->bind_for_final_pass();

	
	}

	


	void DeferredRenderer::destroy(){
		delete m_fbo_deferred;
		delete m_fbo_shadow;
		delete m_bounding_box;
		delete m_fullscreen_quad;
		m_null_program->destroy();
		m_blit_program->destroy();
		m_directional_shadow_program->destroy();
	}


	
}