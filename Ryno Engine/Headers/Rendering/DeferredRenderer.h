#pragma once

#include <string>
#include <GL/glew.h>
#include "Global.h"
#include <vector>
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "FBOs/FBO_Deferred.h"
#include "FBOs/FBO_Shadow.h"
#include "SimpleDrawer.h"
#include "Global.h"
#include "Shader.h"
#include "Batches/Batch3DGeometry.h"
#include "Batches/Batch3DShadow.h"
#include "Batches/Batch2DSprite.h"
#include "Batches/Batch2DFont.h"
#include "GameObject.h"

#define NUM_OF_LAYERS 6
namespace Ryno{

	enum LightType{DIR,POINT,SPOT};

	struct CameraDirection
	{
		GLenum CubemapFace;
		glm::vec3 Target;
		glm::vec3 Up;
	};

	struct DirLightStruct {
		ColorRGBA diffuse;
		ColorRGBA specular;
		ColorRGBA ambient;
		U32 blur;
		float diffuse_intensity;
		float specular_intensity;
		float ambient_intensity;
		float shininess;
		glm::vec4 direction;
	};
	struct PointLightStruct {
		ColorRGBA diffuse;
		ColorRGBA specular;
		float diffuse_intensity;
		float specular_intensity;
		glm::vec4 position; 
		float max_fov; float attenuation; float shininess; float _pad;
	};
	struct SpotLightStruct {
		ColorRGBA diffuse;
		ColorRGBA specular;
		float diffuse_intensity;
		float specular_intensity;
		glm::vec4 position;
		glm::vec4 direction;
		float cutoff; float attenuation; U32 blur; float shininess;
	};


	class Game;
	class DeferredRenderer{
		friend class Shell;
	public:

		
		~DeferredRenderer(){}
		static DeferredRenderer* get_instance();

		bool geometry_enabled = true;
		bool lights_enabled[3]{ true,true,true };
		bool shadows_enabled[3]{ true,true,true };
		bool skybox_enabled = true;
		bool postprocessor_enabled = true;
		bool gui_sprites_enabled = true;
		bool gui_text_enabled = true;


		//Initialize deferred rendering
		void init();

		void init_frame();

		void fill_batches();

		//Call before drawing geometry
		void geometry_pass();

		//Skybox pass. Not sure where to use yet
		void skybox_pass();

		//Pass for point lights
		void point_light_pass();

		//Pass for spot lights
		void spot_light_pass();

		//Pass for directional lights
		void directional_light_pass();
	


		//Post processing effects, GUI not influenced
		void post_processing_pass();

		//GUI pass
		void GUI_pass();



		//Print on screen the result of the whole deferred rendering
		void final_pass();

	
		void destroy();

		void set_camera(Camera3D* camera);

		//BATCHES
		Batch3DGeometry m_geometry_batch3d;
		Batch3DShadow  m_shadow_batch3d;
		Batch2DSprite m_sprite_batch2d;
		Batch2DFont m_font_batch2d;

		struct UBO_Global_Data {
			glm::mat4 V;
			glm::mat4 iV;
			glm::mat4 P;
			glm::mat4 iP;
			glm::mat4 VP;
			glm::mat4 iVP;
			glm::mat4 itV;
			glm::vec4 cameraPos;
			float time;
			U32 screen_width;
			U32 screen_height;
		} ubo_global_data;


		U32 global_ubo = 0;

		U32 compute_light_ssbos[3]{ 0 };
		std::string compute_light_ssbos_names[3]{ "compute_dir_ssbo", "compute_point_ssbo", "compute_spot_ssbo" };
		U32 light_ssbos[3]{ 3 };
		std::string light_ssbos_names[3]{ "dir_ssbo", "point_ssbo", "spot_ssbo" };

		static void bind_global_ubo(const Shader& s) { bind_ubo("glob_ubo", get_instance()->global_ubo, 0,s); }
	private:

		DeferredRenderer() {}

		//Extra dir light passes
		void dir_lighting_subpass(DirLightStruct& ls, DirectionalLight* l, U32 index);
		void dir_shadow_subpass(DirLightStruct& ls, DirectionalLight* l);
		DirLightStruct fillDirLightStruct(DirectionalLight* l) const;

		//Extra point light passes
		void point_lighting_subpass(PointLightStruct& ls, PointLight* l, U32 index);
		void point_shadow_subpass(PointLightStruct& ls, PointLight* l);
		PointLightStruct fillPointLightStruct(PointLight* l) const;

		//Extra spot light passes
		void spot_lighting_subpass(SpotLightStruct& ls, SpotLight* go, U32 index);
		void spot_shadow_subpass(SpotLightStruct& ls, SpotLight* go);
		SpotLightStruct fillSpotLightStruct(SpotLight* l) const;




		
	
		static void bind_ubo(const std::string& name, U32 block, U32 bind_point, const Shader& s);
		static void bind_ssbo(const std::string& name, U32 block, U32 bind_point, const Shader& s);
		

		Camera3D* m_camera;
		FBO_Deferred m_fbo_deferred;
		FBO_Shadow m_fbo_shadow;
		SimpleDrawer* m_simple_drawer;
		Game* game;
		MeshManager* m_mesh_manager;
		TextureManager* m_texture_manager;

		//PROGRAMS
		Shader m_skybox_program,m_flat_program,m_sprite_program,m_font_program;
		Shader m_blit_depth, m_blit_color;
		Shader compute_shaders[3];														
		Shader light_shaders[3];			
		Shader shadow_shaders[3];	
	

		SubModel light_models[3];			
		SubModel m_blit_model_depth, m_blit_model_color, m_skybox_model, m_post_proc_model;
		glm::mat4 bias;
		mutable glm::mat4 light_VP_matrix;
		static const CameraDirection camera_directions[NUM_OF_LAYERS]; 

		template <class T>
		void fill_ssbo(U32 ssbo, std::vector<T>& vec) {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * vec.size(), vec.data(), GL_DYNAMIC_READ);
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
			memcpy(p, vec.data(), sizeof(T) * vec.size());
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		}

		template<class T>
		void tiled_pass(std::vector<T>& lss, LightType type) {

			U32 nrOfLights = lss.size();

			auto& s = compute_shaders[type];
			bind_global_ubo(s);
			bind_ssbo(compute_light_ssbos_names[type], compute_light_ssbos[type], 2, s);

			s.use();
			m_fbo_deferred.bind_fbo();
			U8 samplerIndex = 0;

			s.send_uniform_to_shader("main_tex", &m_fbo_deferred.m_final_textures[0], &samplerIndex);
			s.send_uniform_to_shader("nrOfLights", &nrOfLights, &samplerIndex);
			s.send_uniform_to_shader("diffuse_tex", &m_fbo_deferred.m_textures[0], &samplerIndex);
			s.send_uniform_to_shader("specular_tex", &m_fbo_deferred.m_textures[1], &samplerIndex);
			s.send_uniform_to_shader("normal_tex", &m_fbo_deferred.m_textures[2], &samplerIndex);
			s.send_uniform_to_shader("depth_tex", &m_fbo_deferred.m_textures[3], &samplerIndex);

			const int grid_size = 16;
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			glDispatchCompute(std::ceil(WindowSize::w / grid_size), std::ceil(WindowSize::h / grid_size), 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			s.unuse();
		}
	
};
	
}