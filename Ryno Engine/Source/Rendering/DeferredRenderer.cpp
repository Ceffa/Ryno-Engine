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
		light_models[DIR].mesh = m_blit_model_depth.mesh;
		light_models[POINT].mesh = m_mesh_manager->load_mesh("bound_sphere", ENGINE);
		light_models[SPOT].mesh = m_mesh_manager->load_mesh("bound_pyramid", ENGINE);




		//SHADER PROGRAMS LOADING
		shadow_shaders[DIR].create("ShadowPass/dir_shadow", ENGINE);
		light_shaders[DIR].create("LightPass/dir_light", ENGINE);
		compute_shaders[DIR].create("ComputePass/dir_compute", ENGINE);
		light_models[DIR].material.set_shader(&light_shaders[DIR]);

		shadow_shaders[POINT].create("ShadowPass/point_shadow", ENGINE);
		light_shaders[POINT].create("LightPass/point_light", ENGINE);
		compute_shaders[POINT].create("ComputePass/point_compute", ENGINE);
		light_models[POINT].material.set_shader(&light_shaders[POINT]);

		shadow_shaders[SPOT].create("ShadowPass/spot_shadow", ENGINE);
		light_shaders[SPOT].create("LightPass/spot_light", ENGINE);
		compute_shaders[SPOT].create("ComputePass/spot_compute", ENGINE);
		light_models[SPOT].material.set_shader(&light_shaders[SPOT]);

	
		m_skybox_program.create("SkyboxPass/skybox",ENGINE);
		m_skybox_model.material.set_shader(&m_skybox_program);

		m_blit_depth.create("Others/blit2depth", ENGINE);
		m_blit_model_depth.material.set_shader(&m_blit_depth);
		

		m_blit_color.create("Others/blit2color", ENGINE);
		m_blit_model_color.material.set_shader(&m_blit_color);

		


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
		glGenBuffers(3, compute_light_ssbos);
		glGenBuffers(3, light_ssbos);

	}
	
	void DeferredRenderer::init_frame(){

		//Fill UBO data
		ubo_global_data.V = m_camera->get_V_matrix();
		ubo_global_data.iV = glm::inverse(ubo_global_data.V);
		ubo_global_data.P = m_camera->get_P_matrix();
		ubo_global_data.iP = glm::inverse(ubo_global_data.P);
		ubo_global_data.VP = m_camera->get_VP_matrix();
		ubo_global_data.iVP = glm::inverse(ubo_global_data.VP);
		ubo_global_data.itV = m_camera->get_itV_matrix();
		ubo_global_data.cameraPos = m_camera->position;
		ubo_global_data.time = TimeManager::time;
		ubo_global_data.screen_width = WindowSize::w;
		ubo_global_data.screen_height = WindowSize::h;


		glBindBuffer(GL_UNIFORM_BUFFER, global_ubo);
		GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_ONLY);
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
	


	void DeferredRenderer::directional_light_pass()
	{
		if (!directional_light_enabled)
			return;

		LightType t = DIR;
		//Creates two arrays of lights
		std::vector<DirLightStruct> lights{};
		std::vector<DirectionalLight*> lights_ptr{};
		std::vector<DirLightStruct> computeLights{};

		for (auto l : DirectionalLight::dir_lights){
			if (!l->active || !l->game_object->active)
				continue;
			if (l->shadows && directional_shadow_enabled) {
				lights.emplace_back(fillDirLightStruct(l));
				lights_ptr.push_back(l);
			}
			else {
				computeLights.emplace_back(fillDirLightStruct(l));
			}
		}

		//Fill SSBOs
		fill_ssbo(light_ssbos[t], lights);
		fill_ssbo(compute_light_ssbos[t], computeLights);

		//Process regular ligths
		bind_global_ubo(light_shaders[t]);
		bind_ssbo(light_ssbos_names[t], light_ssbos[t], 1, light_shaders[t]);
		for (U32 i = 0; i < lights.size(); ++i) {
			dir_shadow_subpass(lights[i],lights_ptr[i]);
			dir_lighting_subpass(lights[i],lights_ptr[i], i);
		}
		
		//Process compute lights
		tiled_pass(computeLights,t);
	}	

	void DeferredRenderer::point_light_pass(){		

		if (!point_light_enabled)
			return;

		LightType t = POINT;

		//Creates two arrays of lights
		std::vector<PointLightStruct> lights;
		std::vector<PointLight*> lights_ptr;
		std::vector<PointLightStruct> computeLights;

		for (auto l : PointLight::point_lights) {
			if (!l->active || !l->game_object->active)
				continue;
			if (l->shadows && point_shadow_enabled) {
				lights.emplace_back(fillPointLightStruct(l));
				lights_ptr.push_back(l);
			}
			else {
				computeLights.emplace_back(fillPointLightStruct(l));
			}
		}
		
		//Fill SSBOs
		fill_ssbo(light_ssbos[t], lights);
		fill_ssbo(compute_light_ssbos[t], computeLights);

		//Process regular ligths
		bind_global_ubo(light_shaders[t]);
		bind_ssbo(light_ssbos_names[t], light_ssbos[t], 1, light_shaders[t]);

		for (U32 i = 0; i < lights.size(); ++i) {
			point_shadow_subpass(lights[i],lights_ptr[i]);
			point_lighting_subpass(lights[i],lights_ptr[i], i);
		}

		//Process compute lights
		tiled_pass(computeLights,t);

	}

	void DeferredRenderer::spot_light_pass()
	{
		if (!spot_light_enabled)
			return;

		LightType t = SPOT;

		//Creates two arrays of lights
		std::vector<SpotLightStruct> lights;
		std::vector<SpotLight*> lights_ptr;
		std::vector<SpotLightStruct> computeLights;

		for (auto l : SpotLight::spot_lights) {
			if (!l->active || !l->game_object->active)
				continue;
			if (l->shadows && spot_shadow_enabled) {
				lights.emplace_back(fillSpotLightStruct(l));
				lights_ptr.push_back(l);
			}
			else {
				computeLights.emplace_back(fillSpotLightStruct(l));
			}
		}

		//Fill SSBOs
		fill_ssbo(light_ssbos[t], lights);
		fill_ssbo(compute_light_ssbos[t], computeLights);

		//Process regular ligths
		bind_global_ubo(light_shaders[t]);
		bind_ssbo(light_ssbos_names[t], light_ssbos[t], 1, light_shaders[t]);

		for (U32 i = 0; i < lights.size(); ++i) {
			spot_shadow_subpass(lights[i], lights_ptr[i]);
			spot_lighting_subpass(lights[i], lights_ptr[i], i);
		}

		//Process compute lights
		tiled_pass(computeLights,t);

	}



	DirLightStruct DeferredRenderer::fillDirLightStruct(DirectionalLight* l) const {

		auto go = l->game_object;

		glm::quat rot = l->absolute_movement ? l->rotation : go->transform.get_rotation() * l->rotation;
		Transform* parent = go->transform.get_parent();
		while (parent != nullptr) {
			rot = parent->get_rotation() * rot;
			parent = parent->get_parent();
		}
		DirLightStruct ls;
		ls.direction = glm::vec4(rot * glm::vec3(0, 0, 1),0);
		ls.diffuse = l->diffuse_color;
		ls.specular = l->specular_color;
		ls.ambient = l->ambient_color;
		ls.diffuse_intensity = l->diffuse_intensity;
		ls.specular_intensity = l->specular_intensity;
		ls.ambient_intensity = l->ambient_intensity;
		ls.blur = l->blur;
	
		return ls;
	}

	PointLightStruct DeferredRenderer::fillPointLightStruct(PointLight* l) const{
		
		auto go = l->game_object;
		l->calculate_max_radius();
		glm::vec4 trans = go->transform.hinerited_matrix * go->transform.model_matrix * glm::vec4(0,0,0,1);
		
		
		PointLightStruct ls;
		ls.diffuse = l->diffuse_color;
		ls.specular = l->specular_color;
		ls.diffuse_intensity = l->diffuse_intensity;
		ls.specular_intensity = l->specular_intensity;
		ls.position = trans;
		ls.attenuation = l->attenuation;
		ls.max_fov = l->max_radius;
		return ls;
	}

	SpotLightStruct DeferredRenderer::fillSpotLightStruct(SpotLight* l) const {

		auto go = l->game_object;
		l->calculate_max_radius();
		glm::vec4 trans = go->transform.hinerited_matrix * go->transform.model_matrix * glm::vec4(0, 0, 0, 1);
		glm::quat rot = l->absolute_movement ? l->rotation : go->transform.get_rotation() * l->rotation;
		Transform* parent = go->transform.get_parent();
		while (parent != nullptr) {
			rot = parent->get_rotation() * rot;
			parent = parent->get_parent();
		}
		F32 cutoff_value = cos(l->cutoff * DEG_TO_RAD);

		SpotLightStruct ls;
		ls.diffuse = l->diffuse_color;
		ls.specular = l->specular_color;
		ls.diffuse_intensity = l->diffuse_intensity;
		ls.specular_intensity = l->specular_intensity;
		ls.position = trans;
		ls.cutoff = cutoff_value;
		ls.attenuation = l->attenuation;
		ls.direction = glm::vec4(rot * glm::vec3(0, 0, -1), 0);
		ls.blur = l->blur;
		return ls;
	}



	void DeferredRenderer::dir_lighting_subpass(DirLightStruct& ls, DirectionalLight* l, U32 index)
	{		
		m_fbo_deferred.bind_for_light_pass();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDepthMask(GL_FALSE);


		auto& mat = light_models[DIR].material;

		mat.set_uniform("index", index);

		mat.set_uniform("diffuse_tex", m_fbo_deferred.m_textures[0]);
		mat.set_uniform("specular_tex", m_fbo_deferred.m_textures[1]);
		mat.set_uniform("normal_tex", m_fbo_deferred.m_textures[2]);
		mat.set_uniform("depth_tex", m_fbo_deferred.m_textures[3]);
		mat.set_uniform("shadow_tex", m_fbo_shadow.m_directional_texture);

		mat.set_uniform("jitter", ls.blur == 0 ? m_fbo_shadow.m_jitter[0] : m_fbo_shadow.m_jitter[ls.blur - 1]);
		mat.set_uniform("shadow_strength", l->shadow_strength);
		mat.set_uniform("light_VP_matrix", bias * light_VP_matrix);

		m_simple_drawer->draw(&light_models[DIR]);
	}
	
	void DeferredRenderer::point_lighting_subpass(PointLightStruct& ls,PointLight* l, U32 index){

		m_fbo_deferred.bind_for_light_pass();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		auto& mat = light_models[POINT].material;
		glm::vec3 scale = glm::vec3(l->max_radius);
		glm::quat rot = l->game_object->transform.get_rotation();
		Transform* parent = l->game_object->transform.get_parent();
		while (parent != nullptr) {
			rot = parent->get_rotation() * rot;
			parent = parent->get_parent();
		}
		glm::mat4 model_matrix = glm::scale(
			//Translate matrix
			glm::translate(glm::mat4(1.0f), glm::vec3(ls.position)) *
			//Rotation matrix built from three quaternions
			glm::toMat4(rot),
			//Scaling the rot-trans matrix
			scale);
		mat.set_uniform("diffuse_tex", m_fbo_deferred.m_textures[0]);
		mat.set_uniform("specular_tex", m_fbo_deferred.m_textures[1]);
		mat.set_uniform("normal_tex", m_fbo_deferred.m_textures[2]);
		mat.set_uniform("depth_tex", m_fbo_deferred.m_textures[3]);
		mat.set_uniform("shadow_cube", m_fbo_shadow.m_point_cube);
	
		mat.set_uniform("MVP", m_camera->get_VP_matrix() * model_matrix);
		mat.set_uniform("shadow_strength", l->shadow_strength);
		mat.set_uniform("index", index);

		m_simple_drawer->draw(&light_models[POINT]);

	}

	void DeferredRenderer::spot_lighting_subpass(SpotLightStruct& ls, SpotLight* l, U32 index)
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

		auto go = l->game_object;

		float angle = l->cutoff * DEG_TO_RAD;
		float edge = l->max_radius / cos(angle);
		float width = sin(angle) * edge;
		glm::vec3 scale = glm::vec3(width, width, l->max_radius);

		glm::quat rot = l->absolute_movement ? l->rotation : go->transform.get_rotation() * l->rotation;
		Transform* parent = go->transform.get_parent();
		while (parent != nullptr) {
			rot = parent->get_rotation() * rot;
			parent = parent->get_parent();
		}
		glm::mat4 model_matrix = glm::scale(
			//Translate matrix
			glm::translate(glm::mat4(1.0f), glm::vec3(ls.position)) *
			//Rotation matrix built from three quaternions
			glm::toMat4(rot),
			//Scaling the rot-trans matrix
			scale);




		auto& mat = light_models[SPOT].material;

		mat.set_uniform("diffuse_tex", m_fbo_deferred.m_textures[0]);
		mat.set_uniform("specular_tex", m_fbo_deferred.m_textures[1]);
		mat.set_uniform("normal_tex", m_fbo_deferred.m_textures[2]);
		mat.set_uniform("depth_tex", m_fbo_deferred.m_textures[3]);
		mat.set_uniform("shadow_tex", m_fbo_shadow.m_spot_texture);
		mat.set_uniform("jitter", l->blur == 0 ? m_fbo_shadow.m_jitter[0] : m_fbo_shadow.m_jitter[l->blur - 1]);

		mat.set_uniform("shadow_strength", l->shadow_strength);
		mat.set_uniform("index", index);
		
		mat.set_uniform("MVP", m_camera->get_VP_matrix() * model_matrix);
		mat.set_uniform("light_VP_matrix", bias * light_VP_matrix);
		
		

		bind_global_ubo(*mat.shader);
		m_simple_drawer->draw(&light_models[SPOT]);
	}



	
	void DeferredRenderer::dir_shadow_subpass(DirLightStruct& ls, DirectionalLight* l){

		
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		m_fbo_shadow.bind_for_directional_shadow_pass();

		glClear(GL_DEPTH_BUFFER_BIT);

		//generate light_VP matrix
		auto go = l->game_object;
		glm::mat4 ortho_mat = m_camera->get_O_matrix();
		glm::vec3 dir = glm::vec3(glm::transpose(glm::inverse(l->absolute_movement ? go->transform.hinerited_matrix : go->transform.hinerited_matrix* go->transform.model_matrix)) * (l->rotation * glm::vec4(0, 0, 1, 0)));
		glm::vec3 up_vect = glm::vec3(dir.y, -dir.x, 0);
		light_VP_matrix = ortho_mat * glm::lookAt(dir, glm::vec3(0, 0, 0), up_vect);

		glViewport(0, 0, m_fbo_shadow.directional_resolution, m_fbo_shadow.directional_resolution);

		shadow_shaders[DIR].use();
		glUniformMatrix4fv(shadow_shaders[DIR].getUniformLocation("light_VP"), 1, GL_FALSE, &light_VP_matrix[0][0]);
		m_shadow_batch3d.render_batch();
		shadow_shaders[DIR].unuse();


		glViewport(0, 0, WindowSize::w, WindowSize::h);

	}

	void DeferredRenderer::point_shadow_subpass(PointLightStruct& ls, PointLight* l)
	{
				
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

		
		glm::mat4 light_VP_matrices[NUM_OF_LAYERS];

		glm::mat4 point_shadow_projection_matrix = glm::perspective(HALF_PI, 1.0, 1.0, (F64)ls.max_fov);
		auto pos = glm::vec3(ls.position);

		
		for (U8 i = 0; i < NUM_OF_LAYERS; i++){
		
			//Get view matrix of the light. This is both translate and rotate matrix
			glm::mat4 view_matrix = glm::lookAt(pos,pos + camera_directions[i].Target, camera_directions[i].Up);


			//Multiply view by a perspective matrix large as the light radius
			light_VP_matrices[i] = point_shadow_projection_matrix * view_matrix;
		}

		//Send Vp matrix and world light position to shader, then render
		shadow_shaders[POINT].use();

		glUniformMatrix4fv(shadow_shaders[POINT].getUniformLocation("projection_matrices"),6,GL_FALSE,&light_VP_matrices[0][0][0]);

		m_shadow_batch3d.render_batch();
		shadow_shaders[POINT].unuse();


		//Restore clear color and viewport
		glViewport(0, 0, WindowSize::w, WindowSize::h);
	}

	void DeferredRenderer::spot_shadow_subpass(SpotLightStruct& ls, SpotLight* l)
	{
		GameObject* go = l->game_object;
		//Needs to be done even if shadows disabled
		//Get light position, with correct z axis
		glm::vec3 correct_position = glm::vec3(go->transform.hinerited_matrix * go->transform.model_matrix * glm::vec4(0, 0, 0, 1));
		glm::vec4 dir = glm::transpose(glm::inverse(l->absolute_movement ? go->transform.hinerited_matrix : go->transform.hinerited_matrix* go->transform.model_matrix)) * (l->rotation * glm::vec4(0,0,-1,0));
			
		glm::vec3 up_vector = glm::vec3(dir.y, -dir.x, 0);//One of the perpendicular vectors to the direction
		glm::mat4 view_matrix = glm::lookAt(correct_position, correct_position + glm::vec3(dir), up_vector);
	
		glm::mat4 projection_matrix = glm::perspective( l->cutoff*2 * DEG_TO_RAD, 1.0, 1.0, (F64)l->max_radius);


		//Multiply view by a perspective matrix large as the light radius
		light_VP_matrix = projection_matrix * view_matrix;


		if (!spot_shadow_enabled || !l->shadows)
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
		shadow_shaders[SPOT].use();

		glUniformMatrix4fv(shadow_shaders[SPOT].getUniformLocation("light_VP"), 1, GL_FALSE, &light_VP_matrix[0][0]);

		m_shadow_batch3d.render_batch();
		shadow_shaders[SPOT].unuse();



		//Restore clear color and viewport
		glViewport(0, 0, WindowSize::w, WindowSize::h);
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

		m_blit_model_color.material.set_uniform("source_buffer", m_fbo_deferred.m_final_textures[m_fbo_deferred.m_current_scene_texture]);

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