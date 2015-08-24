#pragma once

#include <string>
#include <GL/glew.h>
#include "Types.h"
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
#define NUM_OF_LAYERS 6
namespace Ryno{

	struct CameraDirection
	{
		GLenum CubemapFace;
		glm::vec3 Target;
		glm::vec3 Up;
	};

	
	
	class DeferredRenderer{
	public:

		//Initialize deferred rendering
		void init(Camera3D* camera);

		void init_frame();

		//Call before drawing geometry
		void geometry_pass(Batch3DGeometry* batch, GLSLProgram* program);

		//Skybox pass. Not sure where to use yet
		void skybox_pass();

		

		//Pass for point lights
		void point_light_pass(std::vector<PointLight*>* point_lights, Batch3DShadow* batch);

		//Pass for spot lights
		void spot_light_pass(std::vector<SpotLight*>* spot_lights, Batch3DShadow* batch);

		//Pass for directional lights
		void directional_lighting_pass(DirectionalLight* directional_light, Batch3DShadow* batch);

		


		//Print on screen the result of the whole deferred rendering
		void final_pass();

	
		void destroy();


	private:

		//Shadow subpass for point light
		void point_shadow_subpass(PointLight* p, Batch3DShadow* batch);

		//Stencil subpass for point light
		void point_stencil_subpass(PointLight* point_light);

		//Lighting subpass for point light
		void point_lighting_subpass(PointLight* point_light);

		//Shadow subpass for spot light
		void spot_shadow_subpass(SpotLight* p, Batch3DShadow* batch);

		//Stencil subpass for spot light
		void spot_stencil_subpass(SpotLight* spot_light);

		//Lighting subpass for spot light
		void spot_lighting_subpass(SpotLight* spot_light);

		//Shadow subpass for directional light 
		void directional_shadow_subpass(DirectionalLight* directional_light, Batch3DShadow* batch);

		//Lighting subpass for directional light
		void directional_lighting_subpass(DirectionalLight* directional_light);

		Camera3D* m_camera;
		FBO_Deferred* m_fbo_deferred;
		FBO_Shadow* m_fbo_shadow;
		SimpleDrawer* m_simple_drawer;
		GLSLProgram* m_null_program, *m_skybox_program, *m_directional_shadow_program, *m_spot_shadow_program, *m_point_shadow_program, *m_blit_program;
		MeshManager* m_mesh_manager;
		TextureManager* m_texture_manager;
		Model* m_bounding_box, *m_fullscreen_quad, *m_cube_box;
		glm::mat4 MVP_camera;
		glm::mat4 inverse_P_matrix;
		glm::mat4 inverse_VP_matrix;
		glm::mat4 spot_VP_matrix;
		glm::mat4 directional_light_VP;
		glm::mat4 bias;
		static const CameraDirection camera_directions[NUM_OF_LAYERS]; 
		

	};
	
}