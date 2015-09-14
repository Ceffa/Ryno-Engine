#pragma once

#include "Global.h"
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
		
		void bind_for_point_shadow_pass();
		void bind_for_point_lighting_pass();
		void bind_for_spot_shadow_pass();
		void bind_for_spot_lighting_pass();
		void bind_for_directional_shadow_pass();
		void bind_for_directional_lighting_pass();

		U32 m_fbo;
		U32 m_directional_texture;
		U32 m_spot_texture;
		U32 m_point_cube;


		U16 point_resolution = 512;
		U16 spot_resolution = 812;
		U16 directional_resolution = 4096;

	private:
		void bind_fbo();

	};
}