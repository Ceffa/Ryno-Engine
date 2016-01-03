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

	struct directional_uniforms_locations{
		I32 direction;
		I32 diffuse;
		I32 specular;
		I32 ambient;
		I32 intensities;
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
		bool gui_sprites_enabled = true;
		bool gui_text_enabled = true;


		//Initialize deferred rendering
		void init();

		void init_frame();

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

		//GUI pass
		void GUI_pass();


		//Print on screen the result of the whole deferred rendering
		void final_pass();

	
		void destroy();

		void set_camera(Camera3D* camera);


	private:

		DeferredRenderer();

		//Shadow subpass for point light
		void point_shadow_subpass(GameObject* go);

		//Lighting subpass for point light
		void point_lighting_subpass(GameObject* go);

		//Shadow subpass for spot light
		void spot_shadow_subpass(GameObject* go);

		//Lighting subpass for spot light
		void spot_lighting_subpass(GameObject* go);

		//Shadow subpass for directional light 
		void directional_shadow_subpass(GameObject* go);

		//Lighting subpass for directional light
		void directional_lighting_subpass(GameObject* go);

	
		Camera3D* m_camera;
		FBO_Deferred* m_fbo_deferred;
		FBO_Shadow* m_fbo_shadow;
		SimpleDrawer* m_simple_drawer;

		//PROGRAMS
		Shader *m_skybox_program;
		Shader *m_directional_shadow_program;
		Shader *m_spot_shadow_program;
		Shader *m_point_shadow_program;
		Shader *m_blit_program;
		Shader *m_point_lighting_program;
		Shader *m_spot_lighting_program;
		Shader *m_flat_program;
		Shader *m_sprite_program;
		Shader *m_font_program;

		//BATCHES
		Batch3DGeometry* m_geometry_batch3d;
		Batch3DShadow* m_shadow_batch3d;
		Batch2DSprite* m_sprite_batch2d;
		Batch2DFont* m_font_batch2d;

		MeshManager* m_mesh_manager;
		TextureManager* m_texture_manager;
		New<Model> m_bounding_sphere, m_fullscreen_quad, m_cube_box, m_bounding_pyramid;
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

		std::vector<GameObject*> point_lights;
		std::vector<GameObject*> spot_lights;
		std::vector<GameObject*> directional_lights;

		

	};
	
}