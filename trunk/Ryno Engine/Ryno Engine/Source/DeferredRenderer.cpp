#include "DeferredRenderer.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtx/string_cast.hpp>

#define PI 3.14159265359
#define HALF_PI 1.57079632679489661923


namespace Ryno{

	const CameraDirection DeferredRenderer::camera_directions[NUM_OF_LAYERS]=
	{
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f) },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
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
		m_null_program->create("null",1,0,1);
		m_skybox_program = new GLSLProgram();
		m_skybox_program->create("skybox",1,0,1);
		m_directional_shadow_program = new GLSLProgram();
		m_directional_shadow_program->create("shadow_directional",1,0,1);
		m_point_shadow_program = new GLSLProgram();
		m_point_shadow_program->create("shadow_point",1,1,1);
		m_spot_shadow_program = new GLSLProgram();
		m_spot_shadow_program->create("shadow_spot", 1, 0, 1);

		m_blit_program = new GLSLProgram();
		m_blit_program->create("blit",1,0,1);
		m_blit_program->use();
		glUniform1i(m_blit_program->getUniformLocation("screen_width"), WINDOW_WIDTH);
		glUniform1i(m_blit_program->getUniformLocation("screen_height"), WINDOW_HEIGHT);
		glUniform1i(m_blit_program->getUniformLocation("source_buffer"), 0);

		m_bounding_sphere = new Model();
		m_bounding_pyramid = new Model();
		m_fullscreen_quad = new Model();
		m_cube_box = new Model();
		m_cube_box->mesh = m_mesh_manager->load_mesh("cubemap_cube",false,ENGINE_FOLDER);
		m_bounding_sphere->mesh = m_mesh_manager->load_mesh("bound_sphere",false,ENGINE_FOLDER);
		m_bounding_pyramid->mesh = m_mesh_manager->load_mesh("bound_pyramid", false,ENGINE_FOLDER);

		m_fullscreen_quad->mesh = m_mesh_manager->load_mesh("square", false,ENGINE_FOLDER);

		bias = glm::mat4(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

	}

	void DeferredRenderer::init_frame(){

		//Calculate camera matrix once and for all
		inverse_P_matrix = glm::inverse(m_camera->get_P_matrix());
		inverse_VP_matrix = glm::inverse(m_camera->get_VP_matrix());



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
		glUniformMatrix4fv(program->getUniformLocation("V"), 1, GL_FALSE, &m_camera->get_V_matrix()[0][0]);
		glUniformMatrix4fv(program->getUniformLocation("VP"), 1, GL_FALSE, &m_camera->get_VP_matrix()[0][0]);
		batch->render_batch();
		program->unuse();
	}

	

	

	//Apply point lights
	void DeferredRenderer::point_light_pass(std::vector<PointLight*>* point_lights, Batch3DShadow* batch){		


		for (PointLight* p : *point_lights){
			point_shadow_subpass(p,batch);
			//point_stencil_subpass(p);
			point_lighting_subpass(p);

		}


	}

	void DeferredRenderer::spot_light_pass(std::vector<SpotLight*>* spot_lights, Batch3DShadow* batch)
	{
		for (SpotLight* p : *spot_lights){
			spot_shadow_subpass(p, batch);
			//spot_stencil_subpass(p);
			spot_lighting_subpass(p);

		}

	}

	void DeferredRenderer::directional_lighting_pass(DirectionalLight* directional_light, Batch3DShadow* batch)
	{
		directional_shadow_subpass(directional_light, batch);
		directional_lighting_subpass(directional_light);
	}

	void DeferredRenderer::point_shadow_subpass(PointLight* p, Batch3DShadow* batch)
	{
		//Enable depth testing and writing
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//Set viewport to cubemap size (because the next rendering will not be at fullscreen)
		glViewport(0, 0, m_fbo_shadow->point_resolution, m_fbo_shadow->point_resolution);
		
		//Bind the whole cubemap, the geometry shader will take care of the faces
		m_fbo_shadow->bind_for_point_shadow_pass();
		glClear(GL_DEPTH_BUFFER_BIT);

		//Get light position, with correct z axis
		glm::vec3 correct_position = glm::vec3(p->position.x, p->position.y, -p->position.z);

		glm::mat4 light_VP_matrices[NUM_OF_LAYERS];
		glm::mat4 point_shadow_projection_matrix = glm::perspective(HALF_PI, 1.0, 1.0, (F64)p->max_radius);

		
		for (U8 i = 0; i < NUM_OF_LAYERS; i++){
		
			//Get view matrix of the light. This is both translate and rotate matrix
			glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + camera_directions[i].Target, camera_directions[i].Up);

			p->calculate_max_radius();

			//Multiply view by a perspective matrix large as the light radius
			light_VP_matrices[i] = point_shadow_projection_matrix * view_matrix;
		}


		//Send Vp matrix and world light position to shader, then render
		m_point_shadow_program->use();

		glUniformMatrix4fv(m_point_shadow_program->getUniformLocation("projection_matrices"),6,GL_FALSE,&light_VP_matrices[0][0][0]);

		batch->render_batch();
		m_point_shadow_program->unuse();

		

	//Restore clear color and viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


	}
	


	

	//Renders point lights inside it's bounding sphere
	void DeferredRenderer::point_lighting_subpass(PointLight* p){

		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_point_lighting_pass();

		//glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glm::vec3 temp_pos = glm::vec3(p->position.x, p->position.y, -p->position.z);
		glm::mat4 scale_box = glm::scale(glm::mat4(1.0f), glm::vec3(p->max_radius));
		glm::mat4 trans_box = glm::translate(glm::mat4(1.0f), temp_pos);

		MVP_camera = m_camera->get_VP_matrix() * trans_box * scale_box;

		p->program->use();
		p->send_uniforms(m_camera);
		glUniform1f(p->program->getUniformLocation("max_fov"),p->max_radius);
		glUniformMatrix4fv(p->program->getUniformLocation("inverse_P_matrix"), 1, GL_FALSE, &inverse_P_matrix[0][0]);
		glUniformMatrix4fv(p->program->getUniformLocation("inverse_VP_matrix"), 1, GL_FALSE, &inverse_VP_matrix[0][0]);
		glUniformMatrix4fv(p->program->getUniformLocation("V_matrix"), 1, GL_FALSE, &m_camera->get_V_matrix()[0][0]);
		glUniformMatrix4fv(p->program->getUniformLocation("MVP"), 1, GL_FALSE, &MVP_camera[0][0]);
		m_simple_drawer->draw(m_bounding_sphere);
		p->program->unuse();

		glDisable(GL_BLEND);
		//glDisable(GL_STENCIL_TEST);

	}


	


	

	


	void DeferredRenderer::spot_shadow_subpass(SpotLight* s, Batch3DShadow* batch)
	{
		
		//Enable depth testing and writing
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//Set viewport to cubemap size (because the next rendering will not be at fullscreen)
		glViewport(0, 0, m_fbo_shadow->spot_resolution, m_fbo_shadow->spot_resolution);

		//Bind the whole cubemap, the geometry shader will take care of the faces
		m_fbo_shadow->bind_for_spot_shadow_pass();
		glClear(GL_DEPTH_BUFFER_BIT);

		//Get light position, with correct z axis
		glm::vec3 correct_position = glm::vec3(s->position.x, s->position.y, -s->position.z);
		
		
		s->calculate_max_radius();
		glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + s->direction, glm::vec3(0, 1, 0));
		glm::mat4 projection_matrix = glm::perspective( s->cutoff*2 * DEG_TO_RAD, 1.0, 1.0, (F64)s->max_radius);


		//Multiply view by a perspective matrix large as the light radius
		spot_VP_matrix = projection_matrix * view_matrix;
		

		//Send Vp matrix and world light position to shader, then render
		m_spot_shadow_program->use();

		glUniformMatrix4fv(m_spot_shadow_program->getUniformLocation("in_VP"), 1, GL_FALSE, &spot_VP_matrix[0][0]);

		batch->render_batch();
		m_spot_shadow_program->unuse();



		//Restore clear color and viewport
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}



	void DeferredRenderer::spot_lighting_subpass(SpotLight* s)
	{
		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_spot_lighting_pass();

		//glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	
		glm::vec3 temp_pos = glm::vec3(s->position.x, s->position.y, -s->position.z);
		float width = s->max_radius *  sin(s->cutoff * DEG_TO_RAD);
		glm::mat4 scale_box = glm::scale(glm::mat4(1.0f), glm::vec3(s->max_radius, s->max_radius, s->max_radius));
		glm::mat4 trans_box = glm::translate(glm::mat4(1.0f), temp_pos);
		glm::mat4 rot_box = glm::toMat4(glm::quat(glm::vec3(0, -s->yaw -M_HALF_PI,0)) * glm::quat(glm::vec3(s->pitch,0, 0)));
	
		MVP_camera = m_camera->get_VP_matrix() * trans_box * rot_box  * scale_box;


		s->program->use();
		s->send_uniforms(m_camera);
		glm::mat4 biased_light_VP_matrix = bias * spot_VP_matrix;
		glUniformMatrix4fv(s->program->getUniformLocation("light_VP_matrix"), 1, GL_FALSE, &biased_light_VP_matrix[0][0]);
		glUniformMatrix4fv(s->program->getUniformLocation("inverse_P_matrix"), 1, GL_FALSE, &inverse_P_matrix[0][0]);
		glUniformMatrix4fv(s->program->getUniformLocation("inverse_VP_matrix"), 1, GL_FALSE, &inverse_VP_matrix[0][0]);
		glUniformMatrix4fv(s->program->getUniformLocation("V_matrix"), 1, GL_FALSE, &m_camera->get_V_matrix()[0][0]);
		glUniformMatrix4fv(s->program->getUniformLocation("MVP"), 1, GL_FALSE, &MVP_camera[0][0]);
		//glUniform1f(spot_light->program->getUniformLocation("max_fov"), spot_light->max_radius);

		m_simple_drawer->draw(m_bounding_pyramid);
		s->program->unuse();

		glDisable(GL_BLEND);
		//glDisable(GL_STENCIL_TEST);
	}

	

	void DeferredRenderer::directional_shadow_subpass(DirectionalLight* directional_light, Batch3DShadow* batch){

		m_fbo_shadow->bind_for_directional_shadow_pass();
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);



		//generate light_VP matrix
		glm::vec3 inv_dir = directional_light->direction;
		glm::mat4 ortho_mat = m_camera->get_O_matrix();
		glm::mat4 view_mat = glm::lookAt(inv_dir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		directional_light_VP = ortho_mat * view_mat;

		glViewport(0, 0, m_fbo_shadow->directional_resolution, m_fbo_shadow->directional_resolution);

		m_directional_shadow_program->use();
		glUniformMatrix4fv(m_directional_shadow_program->getUniformLocation("light_VP"), 1, GL_FALSE, &directional_light_VP[0][0]);
		batch->render_batch();
		m_directional_shadow_program->unuse();


		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);



	}

	//Apply diretional light
	void DeferredRenderer::directional_lighting_subpass(DirectionalLight* directional_light)
	{
		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_directional_lighting_pass();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisable(GL_DEPTH_TEST);

		
		
		glm::mat4 dir_light_VPB = bias * directional_light_VP;

		directional_light->program->use();
		glUniformMatrix4fv(directional_light->program->getUniformLocation("light_VP_matrix"), 1, GL_FALSE, &dir_light_VPB[0][0]);
		glUniformMatrix4fv(directional_light->program->getUniformLocation("inverse_P_matrix"), 1, GL_FALSE, &inverse_P_matrix[0][0]);
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
		glm::mat4 no_trans_VP = m_camera->get_P_matrix() *  glm::mat4(glm::mat3(m_camera->get_V_matrix()));

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
		delete m_bounding_sphere;
		delete m_fullscreen_quad;
		m_null_program->destroy();
		m_blit_program->destroy();
		m_directional_shadow_program->destroy();
	}


	
}