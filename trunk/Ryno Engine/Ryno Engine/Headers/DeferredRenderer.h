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

		//Apply point lights
		void point_light_pass(std::vector<PointLight*>* point_lights);

		//Apply diretional light
		void directional_light_pass(DirectionalLight* directional_light);

		//Print on screen the result of the whole deferred rendering
		void final_pass();

		//Plot on screen the content of the gbuffer buffers
		void debug_geometry_pass();


	private:
		//Stencil pass for point lights only.
		//Call for each light inside the light pass
		void stencil_pass(PointLight* point_light);

		//Renders point light inside it's bounding sphere
		void light_pass(PointLight* point_light);

		Camera3D* m_camera;
		FrameBuffer* m_frame_buffer;
		SimpleDrawer* m_simple_drawer;
		GLSLProgram* m_null_program;
		MeshManager* m_mesh_manager;
		Model* m_bounding_box, *m_fullscreen_quad;
		glm::mat4 MVP_camera;

		

	};
	
}