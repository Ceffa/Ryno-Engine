#include "DeferredRenderer.h"
#include "GameObject.h"
#include "GuiObject.h"

#include "Shell.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtx/string_cast.hpp>

#include "Emitter.h"
#include "Mallocator.h"


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

	

	void DeferredRenderer::set_camera(Camera3D* camera)
	{
		m_camera = camera;
		m_geometry_batch3d->init(m_camera);
		m_shadow_batch3d->init(m_camera);
	}
	void DeferredRenderer::init(){

		//GENERAL SETUP
		m_mesh_manager = MeshManager::get_instance();
		m_texture_manager = TextureManager::get_instance();
		m_simple_drawer = SimpleDrawer::get_instance();

		
		Mallocator* allocator = Mallocator::get_instance();
		m_fbo_deferred.create(allocator,WINDOW_WIDTH, WINDOW_HEIGHT);
		m_fbo_shadow.create(allocator, WINDOW_WIDTH, WINDOW_HEIGHT);

	
		//BATCHES SETUP
		m_geometry_batch3d.create(allocator);
		m_shadow_batch3d.create(allocator);
		m_sprite_batch2d.create(allocator);
		m_font_batch2d.create(allocator);

		m_sprite_batch2d->init();
		m_font_batch2d->init();


	

		//Point shadow program
		m_point_shadow_program.create(allocator);
		m_point_shadow_program->create("ShadowPass/point", ENGINE);
	
		//Spot shadow program
		m_spot_shadow_program.create(allocator);
		m_spot_shadow_program->create("ShadowPass/spot", ENGINE);

		//Directional shadow program
		m_directional_shadow_program.create(allocator);
		m_directional_shadow_program->create("ShadowPass/directional", ENGINE);
	
	
		//Skybox program 
		m_skybox_program.create(allocator);
		m_skybox_program->create("SkyboxPass/skybox",ENGINE);
		
		//Blit program
		m_blit_program.create(allocator);
		m_blit_program->create("Others/blit", ENGINE);
		m_blit_program->use();
		glUniform1i(m_blit_program->getUniformLocation("screen_width"), WINDOW_WIDTH);
		glUniform1i(m_blit_program->getUniformLocation("screen_height"), WINDOW_HEIGHT);
		glUniform1i(m_blit_program->getUniformLocation("source_buffer"), 0);
		m_blit_program->unuse();

		//Sprite program
		m_sprite_program.create(allocator);
		m_sprite_program->create("GUIPass/sprite", ENGINE);
		m_sprite_program->use();
		glUniform1i(m_sprite_program->getUniformLocation("m_texture"), 0);
		m_sprite_program->unuse();

		//Font program
		m_font_program.create(allocator);
		m_font_program->create("GUIPass/font", ENGINE);
		m_font_program->use();
		glUniform1i(m_font_program->getUniformLocation("m_texture"), 0);
		m_font_program->unuse();

		//MODEL LOADING

		m_bounding_sphere.create(allocator);
		m_bounding_sphere->mesh = m_mesh_manager->load_mesh("bound_sphere", false, ENGINE);

		m_bounding_pyramid.create(allocator);
		m_bounding_pyramid->mesh = m_mesh_manager->load_mesh("bound_pyramid", false, ENGINE);

		m_fullscreen_quad.create(allocator);
		m_fullscreen_quad->mesh = m_mesh_manager->load_mesh("square", false, ENGINE);

		m_cube_box.create(allocator);
		m_cube_box->mesh = m_mesh_manager->load_mesh("cubemap_cube", false, ENGINE);

		//BIAS MATRIX
		bias = glm::mat4(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

	}
	
	void DeferredRenderer::init_frame(){

		//Clear lights vectors
		point_lights.clear();
		spot_lights.clear();
		directional_lights.clear();

		//Calculate camera matrix once and for all
		inverse_P_matrix = glm::inverse(m_camera->get_P_matrix());
		inverse_VP_matrix = glm::inverse(m_camera->get_VP_matrix());

		//Setup the two fbos for this frame
		m_fbo_deferred->start_frame();
		m_fbo_shadow->start_frame();

		//Iterate once and for all through the GameObjects
	
		m_shadow_batch3d->begin();
		m_geometry_batch3d->begin();

		for (GameObject* go : GameObject::game_objects)
		{
			go->transform->generate_model_matrix();

			//Fill geometry batch
			if (geometry_enabled){
				if (go->active && *go->model){
					m_geometry_batch3d->draw(*go->model);
					go->model->material.set_attribute("in_M", go->transform->model_matrix);
				}
			}
			//Fill shadow batch
			if (point_shadow_enabled || spot_shadow_enabled || directional_shadow_enabled){
				if (go->active && *go->model)
					m_shadow_batch3d->draw(go);
			}
			//Add ligths
			if (*go->point_light && go->point_light->active)
				point_lights.push_back(go);
			if (*go->spot_light && go->spot_light->active)
				spot_lights.push_back(go);
			if (*go->dir_light && go->dir_light->active)
				directional_lights.push_back(go);

		}
		m_shadow_batch3d->end();
	
		m_geometry_batch3d->end();
	}


	void DeferredRenderer::geometry_pass()
	{
		//Batch was filled in the init method

		m_fbo_deferred->bind_for_geometry_pass();

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		for (Shader* s : m_geometry_batch3d->shaders){
			s->set_global_uniform("g_V", m_camera->get_V_matrix());
			s->set_global_uniform("g_VP", m_camera->get_VP_matrix());
		}
		m_geometry_batch3d->render_batch();
	}
	

	void DeferredRenderer::point_light_pass(){		

		if (!point_light_enabled)
			return;
		for (GameObject* go : point_lights){
			point_shadow_subpass(go);
			point_lighting_subpass(go);

		}


	}


	void DeferredRenderer::spot_light_pass()
	{
		if (!spot_light_enabled)
			return;
		for (GameObject* go : spot_lights){
			spot_shadow_subpass(go);
			spot_lighting_subpass(go);

		}

	}


	void DeferredRenderer::directional_light_pass()
	{
		if (!directional_light_enabled)
			return;
		for (GameObject* go : directional_lights){
			directional_shadow_subpass(go);
			directional_lighting_subpass(go);
		}
	}	


	void DeferredRenderer::point_shadow_subpass(GameObject* go)
	{
		
		if (!point_shadow_enabled)
			return;

		PointLight* p = *go->point_light;

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
		glm::vec3 correct_position = glm::vec3(go->transform->position.x, go->transform->position.y, -go->transform->position.z);

		glm::mat4 light_VP_matrices[NUM_OF_LAYERS];


		p->calculate_max_radius();

		glm::mat4 point_shadow_projection_matrix = glm::perspective(HALF_PI, 1.0, 1.0, (F64)p->max_radius);

		
		for (U8 i = 0; i < NUM_OF_LAYERS; i++){
		
			//Get view matrix of the light. This is both translate and rotate matrix
			glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + camera_directions[i].Target, camera_directions[i].Up);


			//Multiply view by a perspective matrix large as the light radius
			light_VP_matrices[i] = point_shadow_projection_matrix * view_matrix;
		}

		//Send Vp matrix and world light position to shader, then render
		m_point_shadow_program->use();

		glUniformMatrix4fv(m_point_shadow_program->getUniformLocation("projection_matrices"),6,GL_FALSE,&light_VP_matrices[0][0][0]);

		m_shadow_batch3d->render_batch();
		m_point_shadow_program->unuse();

		

	//Restore clear color and viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


	}
	

	void DeferredRenderer::point_lighting_subpass(GameObject* go){

		auto p = *go->point_light;
		auto mod = *p->model;
		mod->mesh = m_bounding_sphere->mesh;
		auto& mat = mod->material;

		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_point_lighting_pass();


		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glm::vec3 light_pos = glm::vec3(go->transform->position.x, go->transform->position.y, -go->transform->position.z);
		glm::mat4 scale_box = glm::scale(glm::mat4(1.0f), glm::vec3(p->max_radius));
		glm::mat4 trans_box = glm::translate(glm::mat4(1.0f), light_pos);

		MVP_camera = m_camera->get_VP_matrix() * trans_box * scale_box;


		//SEND POINT LIGHT UNIFORMS (each for light)
		mat.set_uniform("point_light.position", light_pos);
		mat.set_uniform("point_light.attenuation", p->attenuation);
		mat.set_uniform("point_light.diffuse", p->diffuse_color);
		mat.set_uniform("point_light.specular", p->specular_color);
		mat.set_uniform("point_light.specular_intensity", p->specular_intensity);
		mat.set_uniform("point_light.diffuse_intensity", p->diffuse_intensity);

		//CONSTANT UNIFORMS, IN THE FUTURE MAKE THEM GLOBAL
		mat.set_uniform("screen_width", WINDOW_WIDTH);
		mat.set_uniform("screen_height", WINDOW_HEIGHT);
		mat.set_uniform("color_tex", m_fbo_deferred->m_textures[0]);
		mat.set_uniform("normal_tex", m_fbo_deferred->m_textures[1]);
		mat.set_uniform("depth_tex", m_fbo_deferred->m_textures[2]);
		mat.set_uniform("shadow_cube", m_fbo_shadow->m_point_cube);
	
		//SEND OTHER UNIFORMS
		mat.set_uniform("max_fov", p->max_radius);
		mat.set_uniform("inverse_P_matrix",inverse_P_matrix);
		mat.set_uniform("inverse_VP_matrix", inverse_VP_matrix);
		mat.set_uniform("V_matrix",m_camera->get_V_matrix());
		mat.set_uniform("MVP", MVP_camera);
		mat.set_uniform("shadows_enabled", point_shadow_enabled);

		m_simple_drawer->draw_new(mod);

		glDisable(GL_BLEND);

	}

	
	void DeferredRenderer::spot_shadow_subpass(GameObject* go)
	{

		if (!spot_shadow_enabled)
			return;

		SpotLight* s = *go->spot_light;

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
		glm::vec3 correct_position = glm::vec3(go->transform->position.x, go->transform->position.y, -go->transform->position.z);
		
		
		s->calculate_max_radius();
	
		glm::vec3 up_vector = glm::vec3(s->direction.y, -s->direction.x, 0);//One of the perpendicular vectors to the direction
		glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + s->direction, up_vector);
	
		glm::mat4 projection_matrix = glm::perspective( s->cutoff*2 * DEG_TO_RAD, 1.0, 1.0, (F64)s->max_radius);


		//Multiply view by a perspective matrix large as the light radius
		spot_VP_matrix = projection_matrix * view_matrix;
		

		//Send Vp matrix and world light position to shader, then render
		m_spot_shadow_program->use();

		glUniformMatrix4fv(m_spot_shadow_program->getUniformLocation("in_VP"), 1, GL_FALSE, &spot_VP_matrix[0][0]);

		m_shadow_batch3d->render_batch();
		m_spot_shadow_program->unuse();



		//Restore clear color and viewport
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}


	void DeferredRenderer::spot_lighting_subpass(GameObject* go)
	{
		auto s = *go->spot_light;
		auto mod = *s->model;
		mod->mesh = m_bounding_pyramid->mesh;
		auto& mat = mod->material;

		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_spot_lighting_pass();


		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	
		glm::vec3 light_pos = glm::vec3(go->transform->position.x, go->transform->position.y, -go->transform->position.z);
		float width = s->max_radius *  sin(s->cutoff * DEG_TO_RAD);
		glm::mat4 scale_box = glm::scale(glm::mat4(1.0f), glm::vec3(width, s->max_radius, width));
		glm::mat4 trans_box = glm::translate(glm::mat4(1.0f), light_pos);
		glm::mat4 rot_box = glm::toMat4(glm::quat(glm::vec3(0, -s->yaw -M_HALF_PI,0)) * glm::quat(glm::vec3(s->pitch,0, 0)));
	
		MVP_camera = m_camera->get_VP_matrix() * trans_box * rot_box  * scale_box;
		glm::mat4 biased_light_VP_matrix = bias * spot_VP_matrix;

		F32 cutoff_value = cos(s->cutoff * DEG_TO_RAD);

		//SEND SPOT LIGHT UNIFORMS
		mat.set_uniform("spot_light.position", light_pos);
		mat.set_uniform("spot_light.attenuation", s->attenuation);
		mat.set_uniform("spot_light.direction", s->direction);
		mat.set_uniform("spot_light.cutoff", cutoff_value);
		mat.set_uniform("spot_light.diffuse", s->diffuse_color);
		mat.set_uniform("spot_light.specular",s->specular_color);
		mat.set_uniform("spot_light.diffuse_intensity", s->diffuse_intensity);
		mat.set_uniform("spot_light.specular_intensity", s->specular_intensity);
		mat.set_uniform("screen_width", WINDOW_WIDTH);
		mat.set_uniform("screen_height", WINDOW_HEIGHT);
		mat.set_uniform("color_tex", m_fbo_deferred->m_textures[0]);
		mat.set_uniform("normal_tex", m_fbo_deferred->m_textures[1]);
		mat.set_uniform("depth_tex", m_fbo_deferred->m_textures[2]);
		mat.set_uniform("shadow_tex", m_fbo_shadow->m_spot_texture);

		
		mat.set_uniform("light_VP_matrix", biased_light_VP_matrix);
		mat.set_uniform("inverse_P_matrix", inverse_P_matrix);
		mat.set_uniform("inverse_VP_matrix", inverse_VP_matrix);
		mat.set_uniform("V_matrix",m_camera->get_V_matrix());
		mat.set_uniform("MVP",MVP_camera);
		mat.set_uniform("shadows_enabled", spot_shadow_enabled);

		m_simple_drawer->draw_new(mod);

		glDisable(GL_BLEND);
	}
	

	void DeferredRenderer::directional_shadow_subpass(GameObject* go){

		DirectionalLight* d = *go->dir_light;

		if (!directional_shadow_enabled)
			return;
		m_fbo_shadow->bind_for_directional_shadow_pass();
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);



		//generate light_VP matrix
		glm::mat4 ortho_mat = m_camera->get_O_matrix();
		glm::vec3 up_vect = glm::vec3(d->direction.y, -d->direction.x, 0);
		glm::mat4 view_mat = glm::lookAt(d->direction, glm::vec3(0, 0, 0), up_vect);
		directional_light_VP = ortho_mat * view_mat;

		glViewport(0, 0, m_fbo_shadow->directional_resolution, m_fbo_shadow->directional_resolution);

		m_directional_shadow_program->use();
		glUniformMatrix4fv(m_directional_shadow_program->getUniformLocation("light_VP"), 1, GL_FALSE, &directional_light_VP[0][0]);
		m_shadow_batch3d->render_batch();
		m_directional_shadow_program->unuse();


		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);



	}


	void DeferredRenderer::directional_lighting_subpass(GameObject* go)
	{

		auto d = *go->dir_light;
		auto mod = *d->model;
		mod->mesh = m_fullscreen_quad->mesh;
		auto& mat = mod->material;
		auto s =mat.shader;
		m_fbo_deferred->bind_for_light_pass();
		m_fbo_shadow->bind_for_directional_lighting_pass();

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		
		glm::mat4 dir_light_VPB = bias * directional_light_VP;

		glm::vec3 dir_in_view_space = glm::vec3(glm::transpose(glm::inverse(m_camera->get_V_matrix()))*
			glm::vec4(d->direction, 0));


		mat.set_uniform("screen_width", WINDOW_WIDTH);
		mat.set_uniform("screen_height", WINDOW_HEIGHT);
		mat.set_uniform("color_tex", m_fbo_deferred->m_textures[0]);
		mat.set_uniform("normal_tex", m_fbo_deferred->m_textures[1]);
		mat.set_uniform("depth_tex", m_fbo_deferred->m_textures[2]);
		mat.set_uniform("shadow_tex", m_fbo_shadow->m_directional_texture);
	

		//SEND DIR LIGHT UNIFORMS
		mat.set_uniform("dir_light.direction", dir_in_view_space);
		mat.set_uniform("dir_light.diffuse", d->diffuse_color);
		mat.set_uniform("dir_light.specular", d->specular_color);
		mat.set_uniform("dir_light.ambient", d->ambient_color);
		mat.set_uniform("dir_light.diffuse_intensity", d->diffuse_intensity);
		mat.set_uniform("dir_light.specular_intensity", d->specular_intensity);
		mat.set_uniform("dir_light.ambient_intensity", d->ambient_intensity);

		//SEND OTHER UNIFORMS
		mat.set_uniform("light_VP_matrix",dir_light_VPB);
		mat.set_uniform("inverse_P_matrix",inverse_P_matrix);
		mat.set_uniform("inverse_VP_matrix", inverse_VP_matrix);
		mat.set_uniform("shadows_enabled", directional_shadow_enabled);


		m_simple_drawer->draw_new(mod);

		
		
		glDisable(GL_BLEND);
	}


	void DeferredRenderer::skybox_pass(){
		if (!skybox_enabled)
			return;
		m_fbo_deferred->bind_for_skybox_pass();
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);

		//copy depth buffer (the one created by geometry pass) inside the actual depth buffer to test
		m_blit_program->use();
		m_simple_drawer->draw(*m_fullscreen_quad);
		m_blit_program->unuse();

		glDepthMask(GL_FALSE);
		

		//To draw sky at infinite Z
		glDepthRange(0.99999, 1.0);

		
		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		m_skybox_program->use();

		//Remove translation from VP matrix
		glm::mat4 no_trans_VP = m_camera->get_P_matrix() *  glm::mat4(glm::mat3(m_camera->get_V_matrix()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_camera->skybox.id);
		glUniformMatrix4fv(m_skybox_program->getUniformLocation("no_trans_VP"), 1, GL_FALSE, &no_trans_VP[0][0]);
		glUniform1i(m_skybox_program->getUniformLocation("cube_map"), 0);


		m_simple_drawer->draw(*m_cube_box);

		m_skybox_program->unuse();

		//Restore depth
		glDepthRange(0.0, 1.0);
		glDepthMask(GL_TRUE);



	}



	void DeferredRenderer::GUI_pass()
	{

		m_fbo_deferred->bind_for_GUI_pass();
		glClear(GL_DEPTH_BUFFER_BIT);


		//Add the GUI elements to the 2D batches
		m_sprite_batch2d->begin();
		m_font_batch2d->begin();

		for (GUIObject* go : GUIObject::gui_objects)
		{
			if (*go->sprite && (gui_sprites_enabled || go->sprite->use == SHELL)) {
				go->sprite->generate_model_matrix();
				m_sprite_batch2d->draw(*go->sprite);
			}
			if (*go->text && (gui_text_enabled || go->text->use == SHELL)) {
				m_font_batch2d->draw_font(*go->text);
			}
		}
		

		m_sprite_batch2d->end();
		m_font_batch2d->end();


		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_font_program->use();
		m_font_batch2d->render_batch();
		m_font_program->unuse();
		m_sprite_program->use();
		m_sprite_batch2d->render_batch();
		m_sprite_program->unuse();

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

	}


	void DeferredRenderer::final_pass(){
		m_fbo_deferred->bind_for_final_rendering_pass();

	
	}


	void DeferredRenderer::destroy(){
		
	}


	
}