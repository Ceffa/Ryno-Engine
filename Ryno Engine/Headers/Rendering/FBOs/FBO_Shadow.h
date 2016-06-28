#pragma once

#include "Global.h"
#include "Shader.h"
#include <GLM/glm.hpp>
namespace Ryno{
	class FBO_Shadow
	{
	public:
		FBO_Shadow::FBO_Shadow(U32 width, U32 height);
		FBO_Shadow() {}
		~FBO_Shadow(){}

		void init(U32 width, U32 height);
		
		void start_frame();
		
		void bind_for_point_shadow_pass();
		void bind_for_spot_shadow_pass();
		void bind_for_directional_shadow_pass();

		U32 m_fbo;
		U32 m_directional_texture;
		U32 m_spot_texture;
		U32 m_point_cube;
		U32 m_jitter[3];


		U16 point_resolution = 1024;
		U16 spot_resolution = 512;
		U16 directional_resolution = 2048;

	private:
		void bind_fbo();
		void create_jitter(U8 index, U8 SIZE);
	};
}