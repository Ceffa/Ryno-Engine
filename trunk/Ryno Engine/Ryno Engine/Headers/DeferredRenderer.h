#pragma once

#include <string>
#include <GL/glew.h>
#include "Types.h"
#include <vector>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "FrameBuffer.h"
#include "SimpleDrawer.h"
#include "Global.h"
#include "GLSLProgram.h"
namespace Ryno{
	
	class DeferredRenderer{
	public:

		//Initialize deferred rendering
		void init(Camera3D* camera);

		//Call before drawing geometry
		void init_geometric_pass();

		//Skybox pass. Not sure where to use yet
		void skybox_pass();


		//Apply point lights
		void point_light_pass(std::vector<PointLight*>* point_lights);

		//Apply diretional light
		void directional_light_pass(DirectionalLight* directional_light);

		//Shadow pass for directional light only
		void shadow_pass(DirectionalLight* directional_light);

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
		FrameBuffer* m_frame_buffer;
		SimpleDrawer* m_simple_drawer;
		GLSLProgram* m_null_program, *m_skybox_program, *m_shadow_program, *m_blit_program;
		MeshManager* m_mesh_manager;
		TextureManager* m_texture_manager;
		Model* m_bounding_box, *m_fullscreen_quad, *m_cube_box;
		glm::mat4 MVP_camera;
		glm::mat4 inverse_P;

		

	};
	
}