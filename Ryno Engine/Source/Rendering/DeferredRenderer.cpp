#include "DeferredRenderer.h"
#include "GameObject.h"
#include "GuiObject.h"
#include "Game.h"
#include "Scene.h"

#include "Shell.h"

#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtx/string_cast.hpp>

#include "Emitter.h"



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

	
	
	DeferredRenderer* DeferredRenderer::get_instance() {

		static DeferredRenderer instance;//only at the beginning
		return &instance;
	}

	void DeferredRenderer::set_camera(Camera3D* camera)
	{
		m_camera = camera;
		m_geometry_batch3d.init(m_camera);
		m_shadow_batch3d.init(m_camera);
	}
	void DeferredRenderer::init(){

		game = Game::get_instance();
		//GENERAL SETUP
		m_mesh_manager = MeshManager::get_instance();
		m_texture_manager = TextureManager::get_instance();
		m_simple_drawer = SimpleDrawer::get_instance();

		
		m_fbo_shadow.init(WindowSize::w, WindowSize::h);
		m_fbo_deferred.init(WindowSize::w, WindowSize::h);
	
		m_sprite_batch2d.init();
		m_font_batch2d.init();

	
		//MODEL LOADING

		m_skybox_model.mesh = m_mesh_manager->load_mesh("cubemap_cube", ENGINE);
		m_blit_model_depth.mesh = m_mesh_manager->load_mesh("square", ENGINE);
		m_blit_model_color.mesh = m_blit_model_depth.mesh;
		m_post_proc_model.mesh = m_blit_model_depth.mesh;

		//bounding boxes
		m_dir_bounding.mesh = m_blit_model_depth.mesh;
		m_point_bounding.mesh = m_mesh_manager->load_mesh("bound_sphere", ENGINE);
		m_spot_bounding.mesh = m_mesh_manager->load_mesh("bound_pyramid", ENGINE);




		//SHADER PROGRAMS LOADING

		m_point_shadow_pass.create("ShadowPass/point", ENGINE);
		m_point_light_pass.create("LightPass/point", ENGINE);
		m_point_bounding.material.set_shader(&m_point_light_pass);

	
		m_spot_shadow_pass.create("ShadowPass/spot", ENGINE);
		m_spot_light_pass.create("LightPass/spot", ENGINE);
		m_spot_bounding.material.set_shader(&m_spot_light_pass);

		m_compute_dir.create("ComputePass/dir_compute", ENGINE);
		m_dir_light_pass.create("LightPass/directional", ENGINE);
		m_dir_bounding.material.set_shader(&m_dir_light_pass);
	
		m_skybox_program.create("SkyboxPass/skybox",ENGINE);
		m_skybox_model.material.set_shader(&m_skybox_program);

		m_blit_depth.create("Others/blit2depth", ENGINE);
		m_blit_model_depth.material.set_shader(&m_blit_depth);
		

		m_blit_color.create("Others/blit2color", ENGINE);
		m_blit_model_color.material.set_shader(&m_blit_color);

		m_dir_shadow_pass.create("ShadowPass/directional", ENGINE);
		


		//Sprite program
		m_sprite_program.create("GUIPass/sprite", ENGINE);
		m_sprite_program.use();
		glUniform1i(m_sprite_program.getUniformLocation("m_texture"), 0);
		m_sprite_program.unuse();

		//Font program
		m_font_program.create("GUIPass/font", ENGINE);
		m_font_program.use();
		glUniform1i(m_font_program.getUniformLocation("m_texture"), 0);
		m_font_program.unuse();

		

		//BIAS MATRIX
		bias = glm::mat4(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		//Generat UBOs and SSBOs
		glGenBuffers(1, &global_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, global_ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(UBO_Global_Data), &ubo_global_data, GL_DYNAMIC_DRAW);
		glGenBuffers(1, &dir_light_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, dir_light_ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(DirLightStruct), &dir_light_ubo, GL_DYNAMIC_DRAW);
		glGenBuffers(1, &point_light_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, point_light_ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightStruct), &point_light_ubo, GL_DYNAMIC_DRAW);
		glGenBuffers(1, &spot_light_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, spot_light_ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightStruct), &spot_light_ubo, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glGenBuffers(1, &dir_lights_SSBO);

	}
	
	void DeferredRenderer::init_frame(){

		//Fill UBO data
		ubo_global_data.V = m_camera->get_V_matrix();
		ubo_global_data.iV = glm::inverse(ubo_global_data.V);
		ubo_global_data.P = m_camera->get_P_matrix();
		ubo_global_data.iP = glm::inverse(ubo_global_data.P);
		ubo_global_data.VP = m_camera->get_VP_matrix();
		ubo_global_data.iVP = glm::inverse(ubo_global_data.VP);
		ubo_global_data.cameraPos = m_camera->position;
		ubo_global_data.time = TimeManager::time;
		ubo_global_data.screen_width = WindowSize::w;
		ubo_global_data.screen_height = WindowSize::h;


		glBindBuffer(GL_UNIFORM_BUFFER, global_ubo);
		GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, &ubo_global_data, sizeof(UBO_Global_Data));
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	

		//Setup the two fbos for this frame
		m_fbo_deferred.start_frame();
		m_fbo_shadow.start_frame();

		//Iterate once and for all through the GameObject
		//First generate individual model matrices
		for (GameObject* go : GameObject::game_objects)
			if(go->active)
				go->transform.generate_model_matrix();
		//Then combine them
		for (GameObject* go : GameObject::game_objects)
			if (go->active)
				go->transform.combine_model_matrices();
		
	}


	void DeferredRenderer::fill_batches() {

		m_shadow_batch3d.begin();
		m_geometry_batch3d.begin();

		for (Model* model : Model::models) {
			if (!model->game_object->active)
				continue;
			for (SubModel& s : model->sub_models)
				s.material.set_attribute("in_M", model->game_object->transform.hinerited_matrix * model->game_object->transform.model_matrix);
			m_geometry_batch3d.draw(model);

			if (point_shadow_enabled || spot_shadow_enabled || directional_shadow_enabled)
				m_shadow_batch3d.draw(model);
		}

		m_shadow_batch3d.end();
		m_geometry_batch3d.end();

	}


	void DeferredRenderer::geometry_pass()
	{
		

		m_fbo_deferred.bind_for_geometry_pass();

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_geometry_batch3d.render_batch();

	}
	

	void DeferredRenderer::point_light_pass(){		

		if (!point_light_enabled)
			return;
		int i = 0;
		for (auto* l : PointLight::point_lights){
			if (!l->active || !l->game_object->active)
				continue;
			point_shadow_subpass(l);
			point_lighting_subpass(l);
		}



	}


	void DeferredRenderer::spot_light_pass()
	{
		if (!spot_light_enabled)
			return;
		for (auto* l : SpotLight::spot_lights){
			if (!l->active || !l->game_object->active)
				continue;
			spot_shadow_subpass(l);
			spot_lighting_subpass(l);

		}

	}

	DirLightStruct DeferredRenderer::fillDirLightStruct(DirectionalLight* d) {

		auto go = d->game_object;

		glm::quat rot = d->absolute_movement ? d->rotation : go->transform.get_rotation() * d->rotation;
		Transform* parent = go->transform.get_parent();
		while (parent != nullptr) {
			rot = parent->get_rotation() * rot;
			parent = parent->get_parent();
		}
		DirLightStruct dlc;
		dlc.direction = rot * glm::vec3(0, 0, 1);
		dlc.diffuse = d->diffuse_color;
		dlc.specular = d->specular_color;
		dlc.ambient = d->ambient_color;
		dlc.diffuse_intensity = d->diffuse_intensity;
		dlc.specular_intensity = d->specular_intensity;
		dlc.ambient_intensity = d->ambient_intensity;
		dlc.blur = d->blur;
		dlc.shadow_strength = d->shadow_strength;
		dlc.light_V_matrix = m_camera->get_light_V_matrix();
		if (d->shadows){
			//generate light_VP matrix
			glm::mat4 ortho_mat = m_camera->get_O_matrix();
			glm::vec3 dir = glm::vec3(glm::transpose(glm::inverse(d->absolute_movement ? go->transform.hinerited_matrix : go->transform.hinerited_matrix* go->transform.model_matrix)) * (d->rotation * glm::vec4(0, 0, 1, 0)));
			glm::vec3 up_vect = glm::vec3(dir.y, -dir.x, 0);
			glm::mat4 view_mat = glm::lookAt(dir, glm::vec3(0, 0, 0), up_vect);
			directional_light_VP = ortho_mat * view_mat;
			dlc.light_VP_matrix = bias * directional_light_VP;
		}

		return dlc;
	}

	void DeferredRenderer::directional_light_pass()
	{
		if (!directional_light_enabled)
			return;

		std::vector<DirLightStruct> computeLights;
		for (auto* l : DirectionalLight::dir_lights){
			if (!l->active || !l->game_object->active)
				continue;
			DirLightStruct dlc = fillDirLightStruct(l);
			if (l->shadows && directional_shadow_enabled) {
				directional_shadow_subpass();
				directional_lighting_subpass(dlc);
			}
			else {
				computeLights.emplace_back(dlc);
			}
		}

		directional_light_tiled_pass(computeLights);
	}	

	void DeferredRenderer::point_shadow_subpass(PointLight* p)
	{
		
	
		p->calculate_max_radius();

		if (!point_shadow_enabled || !p->shadows)
			return;


		//Enable depth testing and writing
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//Set viewport to cubemap size (because the next rendering will not be at fullscreen)
		glViewport(0, 0, m_fbo_shadow.point_resolution, m_fbo_shadow.point_resolution);
		
		//Bind the whole cubemap, the geometry shader will take care of the faces
		m_fbo_shadow.bind_for_point_shadow_pass();

		glClear(GL_DEPTH_BUFFER_BIT);

		//Get light position, with correct z axis
		glm::vec3 correct_position = glm::vec3(p->game_object->transform.hinerited_matrix * p->game_object->transform.model_matrix * glm::vec4(0,0,0,1));
		

		glm::mat4 light_VP_matrices[NUM_OF_LAYERS];




		glm::mat4 point_shadow_projection_matrix = glm::perspective(HALF_PI, 1.0, 1.0, (F64)p->max_radius);

		
		for (U8 i = 0; i < NUM_OF_LAYERS; i++){
		
			//Get view matrix of the light. This is both translate and rotate matrix
			glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + camera_directions[i].Target, camera_directions[i].Up);


			//Multiply view by a perspective matrix large as the light radius
			light_VP_matrices[i] = point_shadow_projection_matrix * view_matrix;
		}

		//Send Vp matrix and world light position to shader, then render
		m_point_shadow_pass.use();

		glUniformMatrix4fv(m_point_shadow_pass.getUniformLocation("projection_matrices"),6,GL_FALSE,&light_VP_matrices[0][0][0]);

		m_shadow_batch3d.render_batch();
		m_point_shadow_pass.unuse();

		

	//Restore clear color and viewport
	glViewport(0, 0, WindowSize::w, WindowSize::h);


	}
	

	void DeferredRenderer::point_lighting_subpass(PointLight* p){

		m_fbo_deferred.bind_for_light_pass();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);


		//Generate a special model matrix with the following differences:
		//1) scale is not considered, we use the light one
		//2) translation is precalculated from the hinerited and model matrices, we don't care HOW we get there
		//3) rotation is calculated recursively. This loses info about the axis around which it rotates, bu we don't care,
		//we just need to orient the bounding box

		glm::vec3 trans = glm::vec3(p->game_object->transform.hinerited_matrix * p->game_object->transform.model_matrix * glm::vec4(0, 0, 0, 1));

		glm::vec3 scale = glm::vec3(p->max_radius);
		glm::quat rot = p->game_object->transform.get_rotation();
		Transform* parent = p->game_object->transform.get_parent();
		while (parent != nullptr) {
			rot = parent->get_rotation() * rot;
			parent = parent->get_parent();
		}
		glm::mat4 model_matrix = glm::scale(
			//Translate matrix
			glm::translate(glm::mat4(1.0f), glm::vec3(trans)) *
			//Rotation matrix built from three quaternions
			glm::toMat4(rot),
			//Scaling the rot-trans matrix
			scale);

		MVP_camera = m_camera->get_VP_matrix() * model_matrix;


		auto& mat = m_point_bounding.material;
		//SEND POINT LIGHT UNIFORMS (each for light)
		mat.set_uniform("point_light.position", trans);
		mat.set_uniform("point_light.attenuation", p->attenuation);
		mat.set_uniform("point_light.diffuse", p->diffuse_color);
		mat.set_uniform("point_light.specular", p->specular_color);
		mat.set_uniform("point_light.specular_intensity", p->specular_intensity);
		mat.set_uniform("point_light.diffuse_intensity", p->diffuse_intensity);
		mat.set_uniform("point_light.shadow_strength", p->shadow_strength);


		//CONSTANT UNIFORMS, IN THE FUTURE MAKE THEM glob
		mat.set_uniform("diffuse_tex", m_fbo_deferred.m_textures[0]);
		mat.set_uniform("specular_tex", m_fbo_deferred.m_textures[1]);
		mat.set_uniform("normal_tex", m_fbo_deferred.m_textures[2]);
		mat.set_uniform("depth_tex", m_fbo_deferred.m_textures[3]);
		mat.set_uniform("shadow_cube", m_fbo_shadow.m_point_cube);
	
		//SEND OTHER UNIFORMS
		mat.set_uniform("max_fov", p->max_radius);
		mat.set_uniform("light_V_matrix",m_camera->get_light_V_matrix());
		mat.set_uniform("V_matrix", m_camera->get_V_matrix());


		mat.set_uniform("MVP", MVP_camera);
		mat.set_uniform("shadows_enabled", (point_shadow_enabled && p->shadows) ? 1 : 0);
		
		bind_global_ubo(*mat.shader);
		m_simple_drawer->draw(&m_point_bounding);


		glDisable(GL_BLEND);

	}

	
	void DeferredRenderer::spot_shadow_subpass(SpotLight* s)
	{
		GameObject* go = s->game_object;
		//Needs to be done even if shadows disabled
		//Get light position, with correct z axis
		glm::vec3 correct_position = glm::vec3(go->transform.hinerited_matrix * go->transform.model_matrix * glm::vec4(0, 0, 0, 1));
		glm::vec4 dir = glm::transpose(glm::inverse(s->absolute_movement ? go->transform.hinerited_matrix : go->transform.hinerited_matrix* go->transform.model_matrix)) * (s->rotation * glm::vec4(0,0,-1,0));
		
		s->calculate_max_radius();
	
		glm::vec3 up_vector = glm::vec3(dir.y, -dir.x, 0);//One of the perpendicular vectors to the direction
		glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + glm::vec3(dir), up_vector);
	
		glm::mat4 projection_matrix = glm::perspective( s->cutoff*2 * DEG_TO_RAD, 1.0, 1.0, (F64)s->max_radius);


		//Multiply view by a perspective matrix large as the light radius
		spot_VP_matrix = projection_matrix * view_matrix;


		if (!spot_shadow_enabled || !s->shadows)
			return;


		//Enable depth testing and writing
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		//Set viewport to cubemap size (because the next rendering will not be at fullscreen)
		glViewport(0, 0, m_fbo_shadow.spot_resolution, m_fbo_shadow.spot_resolution);

		//Bind the whole cubemap, the geometry shader will take care of the faces
		m_fbo_shadow.bind_for_spot_shadow_pass();
		glClear(GL_DEPTH_BUFFER_BIT);

		

		//Send Vp matrix and world light position to shader, then render
		m_spot_shadow_pass.use();

		glUniformMatrix4fv(m_spot_shadow_pass.getUniformLocation("light_VP"), 1, GL_FALSE, &spot_VP_matrix[0][0]);

		m_shadow_batch3d.render_batch();
		m_spot_shadow_pass.unuse();



		//Restore clear color and viewport
		glViewport(0, 0, WindowSize::w, WindowSize::h);
	}


	void DeferredRenderer::spot_lighting_subpass(SpotLight* s)
	{
		
		m_fbo_deferred.bind_for_light_pass();


		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	
		//Generate a special model matrix with the following differences:
		//1) scale is not considered, we use the light one
		//2) translation is precalculated from the hinerited and model matrices, we don't care HOW we get there
		//3) rotation is calculated recursively. This loses info about the axis around which it rotates, bu we don't care,
		//we just need to orient the bounding box

		auto go = s->game_object;
		glm::vec3 trans = glm::vec3(go->transform.hinerited_matrix * go->transform.model_matrix * glm::vec4(0, 0, 0, 1));
		float width = s->max_radius *  sin(s->cutoff * DEG_TO_RAD);
		glm::vec3 scale = glm::vec3(width, s->max_radius, width);
		glm::quat rot = s->absolute_movement ?  s-> rotation : go->transform.get_rotation() * s->rotation;
		Transform* parent = go->transform.get_parent();
		while (parent != nullptr) {
			rot = parent->get_rotation() * rot;
			parent = parent->get_parent();
		}
		glm::mat4 model_matrix = glm::scale(
			//Translate matrix
			glm::translate(glm::mat4(1.0f), glm::vec3(trans)) *
			//Rotation matrix built from three quaternions
			glm::toMat4(rot),
			//Scaling the rot-trans matrix
			scale);

		MVP_camera = m_camera->get_VP_matrix() * model_matrix;
		glm::mat4 biased_light_VP_matrix = bias * spot_VP_matrix;

		F32 cutoff_value = cos(s->cutoff * DEG_TO_RAD);

		auto& mat = m_spot_bounding.material;
		//SEND SPOT LIGHT UNIFORMS
		mat.set_uniform("spot_light.position", trans);
		mat.set_uniform("spot_light.attenuation", s->attenuation);
		mat.set_uniform("spot_light.direction", rot * glm::vec3(0,0,-1));
		mat.set_uniform("spot_light.cutoff", cutoff_value);
		mat.set_uniform("spot_light.diffuse", s->diffuse_color);
		mat.set_uniform("spot_light.specular",s->specular_color);
		mat.set_uniform("spot_light.diffuse_intensity", s->diffuse_intensity);
		mat.set_uniform("spot_light.specular_intensity", s->specular_intensity);
		mat.set_uniform("spot_light.blur", s->blur);
		mat.set_uniform("spot_light.shadow_strength", s->shadow_strength);

		mat.set_uniform("diffuse_tex", m_fbo_deferred.m_textures[0]);
		mat.set_uniform("specular_tex", m_fbo_deferred.m_textures[1]);
		mat.set_uniform("normal_tex", m_fbo_deferred.m_textures[2]);
		mat.set_uniform("depth_tex", m_fbo_deferred.m_textures[3]);
		mat.set_uniform("shadow_tex", m_fbo_shadow.m_spot_texture);
		mat.set_uniform("jitter", s->blur == 0 ? m_fbo_shadow.m_jitter[0] : m_fbo_shadow.m_jitter[s->blur - 1]);


		
		mat.set_uniform("light_VP_matrix", biased_light_VP_matrix);
		mat.set_uniform("V_matrix", m_camera->get_V_matrix());
		mat.set_uniform("light_V_matrix", m_camera->get_light_V_matrix());
		
		mat.set_uniform("MVP",MVP_camera);
		mat.set_uniform("shadows_enabled", (spot_shadow_enabled && s->shadows) ? 1 : 0);

		bind_global_ubo(*mat.shader);
		m_simple_drawer->draw(&m_spot_bounding);

		glDisable(GL_BLEND);
	}
	

	void DeferredRenderer::directional_shadow_subpass(){

		
		m_fbo_shadow.bind_for_directional_shadow_pass();
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		

		glViewport(0, 0, m_fbo_shadow.directional_resolution, m_fbo_shadow.directional_resolution);

		m_dir_shadow_pass.use();
		glUniformMatrix4fv(m_dir_shadow_pass.getUniformLocation("light_VP"), 1, GL_FALSE, &directional_light_VP[0][0]);
		m_shadow_batch3d.render_batch();
		m_dir_shadow_pass.unuse();


		glViewport(0, 0, WindowSize::w, WindowSize::h);

	}


	void DeferredRenderer::directional_lighting_subpass(DirLightStruct& dlc)
	{		
		m_fbo_deferred.bind_for_light_pass();

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		
		
		auto& mat = m_dir_bounding.material;

		mat.set_uniform("diffuse_tex", m_fbo_deferred.m_textures[0]);
		mat.set_uniform("specular_tex", m_fbo_deferred.m_textures[1]);
		mat.set_uniform("normal_tex", m_fbo_deferred.m_textures[2]);
		mat.set_uniform("depth_tex", m_fbo_deferred.m_textures[3]);
		mat.set_uniform("shadow_tex", m_fbo_shadow.m_directional_texture);
		mat.set_uniform("jitter", dlc.blur == 0 ? m_fbo_shadow.m_jitter[0] : m_fbo_shadow.m_jitter[dlc.blur-1]);


		//SEND DIR LIGHT UNIFORMS
		glBindBuffer(GL_UNIFORM_BUFFER, dir_light_ubo);
		GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, &dlc, sizeof(DirLightStruct));
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		bind_global_ubo(*mat.shader);
		bind_ubo("dir_ubo", dir_light_ubo, 1, *mat.shader);
		m_simple_drawer->draw(&m_dir_bounding);

		
		glDisable(GL_BLEND);
	}


	void DeferredRenderer::directional_light_tiled_pass(std::vector<DirLightStruct>& dlcs) {

		U32 nrOfLights = dlcs.size();
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, dir_lights_SSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(DirLightStruct) * nrOfLights, dlcs.data(), GL_DYNAMIC_READ);
		GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		memcpy(p, dlcs.data(), sizeof(DirLightStruct) * nrOfLights);

		bind_global_ubo(m_compute_dir);
		bind_ssbo("dir_ssbo", dir_lights_SSBO, 1, m_compute_dir);

		m_compute_dir.use();
		m_fbo_deferred.bind_fbo();
		U8 samplerIndex = 0;

		m_compute_dir.send_uniform_to_shader("main_tex", &m_fbo_deferred.m_final_textures[0],&samplerIndex);
		m_compute_dir.send_uniform_to_shader("nrOfLights", &nrOfLights, &samplerIndex);
		m_compute_dir.send_uniform_to_shader("diffuse_tex", &m_fbo_deferred.m_textures[0], &samplerIndex);
		m_compute_dir.send_uniform_to_shader("specular_tex", &m_fbo_deferred.m_textures[1], &samplerIndex);
		m_compute_dir.send_uniform_to_shader("normal_tex", &m_fbo_deferred.m_textures[2], &samplerIndex);
		m_compute_dir.send_uniform_to_shader("depth_tex", &m_fbo_deferred.m_textures[3], &samplerIndex);
		

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		glDispatchCompute(std::ceil(WindowSize::w/32.0f), std::ceil(WindowSize::h / 32.0f), 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		m_compute_dir.unuse();

		
		

	}

	void DeferredRenderer::skybox_pass(){
		if (!skybox_enabled)
			return;
		m_fbo_deferred.bind_for_skybox_pass();
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
		auto& mat = m_blit_model_depth.material;
		mat.set_uniform("source_buffer", m_fbo_deferred.m_textures[3]);
	
		//copy depth buffer (the one created by geometry pass) inside the actual depth buffer to test
		
		m_simple_drawer->draw(&m_blit_model_depth);

		glDepthMask(GL_FALSE);
		

		//To draw sky at infinite Z
		glDepthRange(0.99999, 1.0);

		
		glDrawBuffer(GL_COLOR_ATTACHMENT4);


		auto& mat2 = m_skybox_model.material;
		mat2.set_uniform("cube_map", m_camera->skybox.id);
		mat2.set_uniform("background", m_camera->background);
		mat2.set_uniform("have_skybox", m_camera->have_skybox ? 1 : 0);

		bind_global_ubo(*mat2.shader);
		m_simple_drawer->draw(&m_skybox_model);

		//Restore depth
		glDepthRange(0.0, 1.0);


	}

	void DeferredRenderer::post_processing_pass() {

		if (!postprocessor_enabled)
			return;
		m_fbo_deferred.m_current_scene_texture = 0; //the fbo is going to start the ping-pong of texture, set the entry point


		glDisable(GL_BLEND);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);

		for (auto& m : game->scene->post_processor->effects) {
			m_post_proc_model.material = m;
			m_fbo_deferred.bind_for_post_processing();
			m.set_uniform("diffuse_tex", m_fbo_deferred.m_textures[0]);
			m.set_uniform("specular_tex", m_fbo_deferred.m_textures[1]);
			m.set_uniform("normal_tex", m_fbo_deferred.m_textures[2]);
			m.set_uniform("depth_tex", m_fbo_deferred.m_textures[3]);
			m.set_uniform("scene_tex", m_fbo_deferred.m_final_textures[1 - m_fbo_deferred.m_current_scene_texture]);
			
			bind_global_ubo(*m.shader);
			m_simple_drawer->draw(&m_post_proc_model);

		}
	}



	void DeferredRenderer::GUI_pass()
	{

		m_fbo_deferred.bind_for_GUI_pass();
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);


		//Add the GUI elements to the 2D batches
		m_sprite_batch2d.begin();
		m_font_batch2d.begin();

		for (GUIObject* go : GUIObject::gui_objects)
		{
			if (go->sprite && (gui_sprites_enabled || go->sprite->use == SHELL)) {
				go->sprite->generate_model_matrix();
				m_sprite_batch2d.draw(go->sprite);
			}
			if (go->text && (gui_text_enabled || go->text->use == SHELL)) {
				m_font_batch2d.draw_font(go->text);
			}
		}
		

		m_sprite_batch2d.end();
		m_font_batch2d.end();


		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_font_program.use();
		m_font_batch2d.render_batch();
		m_font_program.unuse();
		m_sprite_program.use();
		m_sprite_batch2d.render_batch();
		m_sprite_program.unuse();

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

	}


	void DeferredRenderer::final_pass(){
		m_fbo_deferred.bind_for_blit();
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);

		//m_blit_model_color.material.set_uniform("source_buffer", m_fbo_deferred.m_final_textures[m_fbo_deferred.m_current_scene_texture]);
		m_blit_model_color.material.set_uniform("source_buffer", m_fbo_deferred.m_final_textures[0]);

		//copy depth buffer (the one created by geometry pass) inside the actual depth buffer to test
		auto& m = m_blit_model_color.material;
		m_simple_drawer->draw(&m_blit_model_color);
	}


	void DeferredRenderer::destroy() {
	}


	void DeferredRenderer::bind_ubo(const std::string& name, U32 block, U32 bind_point, const Shader& s)
	{
		unsigned int block_index = glGetUniformBlockIndex(s.get_id(), name.c_str());
		glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, block);
		glUniformBlockBinding(s.get_id(), block_index, bind_point);
	}

	void DeferredRenderer::bind_ssbo(const std::string& name, U32 block, U32 bind_point, const Shader& s)
	{
		unsigned int block_index = glGetUniformBlockIndex(s.get_id(), name.c_str());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind_point, block);
		glUniformBlockBinding(s.get_id(), block_index, bind_point);
	}

	


	
}