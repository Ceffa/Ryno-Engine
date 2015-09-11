#pragma once

#include <string>
#include <GL/glew.h>
#include "Global.h"
#include <vector>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "FBO_Deferred.h"
#include "FBO_Shadow.h"
#include "SimpleDrawer.h"
#include "Global.h"
#include "GLSLProgram.h"
#include "Batch3DGeometry.h"
#include "Batch3DShadow.h"
#include "Batch2DSprite.h"
#include "Batch2DFont.h"

#define NUM_OF_LAYERS 6
namespace Ryno{

	struct CameraDirection
	{
		GLenum CubemapFace;
		glm::vec3 Target;
		glm::vec3 Up;
	};

	struct directional_uniforms_locations{
		I32 direction;
		I32 diffuse;
		I32 specular;
		I32 ambient;
	} ;

	struct point_uniforms_locations{
		I32 position;
		I32 diffuse;
		I32 specular;
	};

	struct spot_uniforms_locations{
		I32 position;
		I32 direction;
		I32 diffuse;
		I32 specular;
	};

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
		bool hud_sprites_enabled = true;
		bool hud_text_enabled = true;


		//Initialize deferred rendering
		void init(Camera3D* camera);

		void init_frame();

		//Call before drawing geometry
		void geometry_pass();

		//Skybox pass. Not sure where to use yet
		void skybox_pass();

		//Fill shadow batches
		void prepare_for_light_passes();

		//Pass for point lights
		void point_light_pass();

		//Pass for spot lights
		void spot_light_pass();

		//Pass for directional lights
		void directional_light_pass();

		//HUD pass
		void draw_HUD_pass();


		//Print on screen the result of the whole deferred rendering
		void final_pass();

	
		void destroy();


	private:

		DeferredRenderer();

		//Shadow subpass for point light
		void point_shadow_subpass(PointLight* p);

		//Lighting subpass for point light
		void point_lighting_subpass(PointLight* point_light);

		//Shadow subpass for spot light
		void spot_shadow_subpass(SpotLight* p);

		//Lighting subpass for spot light
		void spot_lighting_subpass(SpotLight* spot_light);

		//Shadow subpass for directional light 
		void directional_shadow_subpass(DirectionalLight* directional_light);

		//Lighting subpass for directional light
		void directional_lighting_subpass(DirectionalLight* directional_light);

	
		Camera3D* m_camera;
		FBO_Deferred* m_fbo_deferred;
		FBO_Shadow* m_fbo_shadow;
		SimpleDrawer* m_simple_drawer;

		//PROGRAMS
		GLSLProgram *m_skybox_program;
		GLSLProgram *m_directional_shadow_program;
		GLSLProgram *m_spot_shadow_program;
		GLSLProgram *m_point_shadow_program;
		GLSLProgram *m_blit_program;
		GLSLProgram *m_geometry_program;
		GLSLProgram *m_directional_lighting_program;
		GLSLProgram *m_point_lighting_program;
		GLSLProgram *m_spot_lighting_program;
		GLSLProgram *m_flat_program;
		GLSLProgram *m_sprite_program;
		GLSLProgram *m_font_program;

		//BATCHES
		Batch3DGeometry* m_geometry_batch3d;
		Batch3DShadow* m_shadow_batch3d;
		Batch2DSprite* m_sprite_batch2d;
		Batch2DFont* m_font_batch2d;

		MeshManager* m_mesh_manager;
		TextureManager* m_texture_manager;
		Model* m_bounding_sphere, *m_fullscreen_quad, *m_cube_box, *m_bounding_pyramid;
		glm::mat4 MVP_camera;
		glm::mat4 inverse_P_matrix;
		glm::mat4 inverse_VP_matrix;
		glm::mat4 spot_VP_matrix;
		glm::mat4 directional_light_VP;
		glm::mat4 bias;
		static const CameraDirection camera_directions[NUM_OF_LAYERS]; 
		spot_uniforms_locations spot_uni_loc;
		point_uniforms_locations point_uni_loc;
		directional_uniforms_locations directional_uni_loc;
		

	};
	
}