#include "DeferredRenderer.h"
#include <GLM/gtx/transform.hpp>


namespace Ryno{

	//Initialize deferred rendering
	void DeferredRenderer::init(Camera3D* camera){

		m_camera = camera;
		m_mesh_manager = MeshManager::get_instance();
		m_simple_drawer = SimpleDrawer::get_instance();
		m_frame_buffer = new FrameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_null_program = new GLSLProgram();
		m_null_program->create("null");
		m_bounding_box = new Model();
		m_fullscreen_quad = new Model();
		m_bounding_box->mesh = m_mesh_manager->load_mesh("bound_sphere");
		m_fullscreen_quad->mesh = m_mesh_manager->load_mesh("square");

	}

	//Call before drawing geometry
	void DeferredRenderer::init_geometric_pass(){
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

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glm::vec3 z = point_light->move_to_view_space(m_camera);
		glm::vec4 color = point_light->get_diffuse_color();
		glUniform3f(point_light->program->getUniformLocation("position"), z.x, z.y, z.z);
		glUniform1f(point_light->program->getUniformLocation("attenuation"), point_light->attenuation);
		glUniform3f(point_light->program->getUniformLocation("color"), color.r, color.g, color.b);
		glUniform1f(point_light->program->getUniformLocation("intensity"), point_light->diffuse_intensity);

		glUniformMatrix4fv(point_light->program->getUniformLocation("MVP"), 1, GL_FALSE, &MVP_camera[0][0]);

		m_simple_drawer->draw(m_bounding_box);
		glCullFace(GL_BACK);

		glDisable(GL_BLEND);
		point_light->program->unuse();
	}


	//Apply diretional light
	void DeferredRenderer::directional_light_pass(DirectionalLight* directional_light)
	{
		glDisable(GL_CULL_FACE);
		m_frame_buffer->bind_for_light_pass();
		directional_light->program->use();
		glDisable(GL_DEPTH_TEST);


		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glm::vec3 r = directional_light->move_to_view_space(m_camera);
		glm::vec4 color = directional_light->get_diffuse_color();
		glUniform3f(directional_light->program->getUniformLocation("dir_light"), r.x, r.y, r.z);
		glUniform3f(directional_light->program->getUniformLocation("dir_color"), color.r, color.g, color.b);
		glUniform1f(directional_light->program->getUniformLocation("dir_intensity"), directional_light->diffuse_intensity);
		
		m_simple_drawer->draw(m_fullscreen_quad);

		glDisable(GL_BLEND);

		//The draw is done, unuse the program
		directional_light->program->unuse();
	}

	//Print on screen the result of the whole deferred rendering
	void DeferredRenderer::final_pass(){
		m_frame_buffer->bind_for_final_pass();
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glDisable(GL_CULL_FACE);
	}


	
}