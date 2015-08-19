#pragma once

#include "Types.h"
#include "GLSLProgram.h"
#include <GLM/glm.hpp>
namespace Ryno{
	class FBO_Shadow
	{
	public:
		FBO_Shadow::FBO_Shadow(U32 width, U32 height);
		~FBO_Shadow(){}

		void init(U32 width, U32 height);

		void start_frame();
		void bind_for_shadow_map_pass();
		void bind_for_directional_light_pass();
		void bind_for_point_light_pass();
		void bind_face(GLenum cube_face);
		void blit_to_debug(U8 face_index);

		U32 m_fbo;
		U32 m_shadow_texture;
		U32 m_shadow_cube;

		glm::mat4 point_shadow_projection_matrix;

		U32 cube_shadow_resolution = 1000;
	private:
		void bind_fbo();

	};
}