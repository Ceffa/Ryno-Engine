#pragma once

#include "Types.h"
#include "GLSLProgram.h"
namespace Ryno{
	class FBO_Shadow
	{
	public:
		FBO_Shadow::FBO_Shadow(U32 width, U32 height);
		~FBO_Shadow(){}

		void init(U32 width, U32 height);

		void start_frame();
		void bind_for_shadow_map_pass();
		void bind_for_light_pass();
		void plot_on_screen();

		U32 m_fbo;
		U32 m_depth_texture;
		U32 m_shadow_texture; //Holds shadow mapping informations

	};
}