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

	struct CameraDirection
	{
		GLenum CubemapFace;
		glm::vec3 Target;
		glm::vec3 Up;
	};

	struct DirLightStruct {
		glm::mat4 light_VP_matrix;
		glm::mat4 light_V_matrix;
		ColorRGBA diffuse;
		ColorRGBA specular;
		ColorRGBA ambient;
		U32 blur;
		float diffuse_intensity;
		float specular_intensity;
		float ambient_intensity;
		float shadow_strength;
		glm::vec3 direction;float _pad;
	};
	struct PointLightStruct {
		
	};
	struct SpotLightStruct {

	};


	class Game;
	class DeferredRenderer{
		friend class Shell;
	public:

		
		~DeferredRenderer(){}
		static DeferredRenderer* get_instance();

		bool geometry_enabled = true;
		bool directional_light_enabled = true;
		bool directional_shadow_enabled = true;
		bool point_light_enabled = true;
		bool point_shadow_enabled = true;
		bool spot_light_enabled = true;
		bool spot_shadow_enabled = true;
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
		void directional_light_tiled_pass(std::vector<DirLightStruct>& dlcs);
		DirLightStruct fillDirLightStruct(DirectionalLight* d);


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
			glm::vec4 cameraPos;
			float time;
			U32 screen_width;
			U32 screen_height;
		} ubo_global_data;


		U32 global_ubo = 0;

		U32 dir_light_ubo = 0;
		U32 point_light_ubo = 0;
		U32 spot_light_ubo = 0;

		GLuint dir_lights_SSBO = 0;
		GLuint point_lights_SSBO = 0;
		GLuint spot_lights_SSBO = 0;

	private:

		DeferredRenderer() {}

		//Shadow subpass for point light
		void point_shadow_subpass(PointLight* go);

		//Lighting subpass for point light
		void point_lighting_subpass(PointLight* go);

		//Shadow subpass for spot light
		void spot_shadow_subpass(SpotLight* go);

		//Lighting subpass for spot light
		void spot_lighting_subpass(SpotLight* go);

		//Shadow subpass for directional light 
		void directional_shadow_subpass();

		//Lighting subpass for directional light
		void directional_lighting_subpass(DirLightStruct& dlc);

	
		static void bind_ubo(const std::string& name, U32 block, U32 bind_point, const Shader& s);
		static void bind_ssbo(const std::string& name, U32 block, U32 bind_point, const Shader& s);

		Camera3D* m_camera;
		FBO_Deferred m_fbo_deferred;
		FBO_Shadow m_fbo_shadow;
		SimpleDrawer* m_simple_drawer;
		Game* game;

		//PROGRAMS
		Shader m_skybox_program,m_flat_program,m_sprite_program,m_font_program;
		Shader m_blit_depth, m_blit_color;
		Shader m_compute_dir;													//compute lights
		Shader m_dir_light_pass, m_point_light_pass, m_spot_light_pass;			//lights
		Shader m_dir_shadow_pass, m_point_shadow_pass, m_spot_shadow_pass;	//Shadows
	

		MeshManager* m_mesh_manager;
		TextureManager* m_texture_manager;
		SubModel m_point_bounding,  m_spot_bounding, m_dir_bounding;			//Bounding models
		SubModel m_blit_model_depth, m_blit_model_color, m_skybox_model, m_post_proc_model;
		glm::mat4 MVP_camera;
		glm::mat4 spot_VP_matrix;
		glm::mat4 directional_light_VP;
		glm::mat4 bias;
		static const CameraDirection camera_directions[NUM_OF_LAYERS]; 

	};
	
}