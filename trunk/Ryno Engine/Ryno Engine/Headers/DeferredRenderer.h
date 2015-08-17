#pragma once

#include <string>
#include <GL/glew.h>
#include "Types.h"
#include <vector>
#include "DirectionalLight.h"
#include "PointLight.h"
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

		//Shadow pass for point lights
		void point_shadow_pass(std::vector<PointLight*>* point_lights, Batch3DShadow* batch);

		//Apply point lights
		void point_light_pass(std::vector<PointLight*>* point_lights);

		//Shadow pass for directional light only
		void directional_shadow_pass(DirectionalLight* directional_light, Batch3DShadow* batch);

		//Apply diretional light
		void directional_light_pass(DirectionalLight* directional_light);


		//Print on screen the result of the whole deferred rendering
		void final_pass();

	
		void destroy();


	private:
		//Stencil pass for point lights only.
		//Call for each light inside the light pass
		void stencil_pass(PointLight* point_light);

		//Renders point light inside it's bounding sphere
		void light_pass(PointLight* point_light);

		Camera3D* m_camera;
		FBO_Deferred* m_fbo_deferred;
		FBO_Shadow* m_fbo_shadow;
		SimpleDrawer* m_simple_drawer;
		GLSLProgram* m_null_program, *m_skybox_program, *m_directional_shadow_program, *m_point_shadow_program, *m_blit_program;
		MeshManager* m_mesh_manager;
		TextureManager* m_texture_manager;
		Model* m_bounding_box, *m_fullscreen_quad, *m_cube_box;
		glm::mat4 MVP_camera;
		glm::mat4 inverse_P;

		static const CameraDirection camera_directions[NUM_OF_LAYERS]; 

		

	};
	
}