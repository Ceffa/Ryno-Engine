#pragma once

#include "Types.h"
#include "GLSLProgram.h"
namespace Ryno{
	class FrameBuffer
	{
	public:
		FrameBuffer(U32 width, U32 height);
		~FrameBuffer(){}
		enum FRAME_TEXTURE_TYPE{
			FRAME_TEXTURE_TYPE_DIFFUSE,
			FRAME_TEXTURE_TYPE_NORMAL,
			FRAME_TEXTURE_TYPE_DEPTH,
			FRAME_NUM_TEXTURES
		};

		void init(U32 width, U32 height);

		void start_frame();
		void bind_for_geometry_pass();
		void bind_for_stencil_pass();
		void bind_for_light_pass();
		void bind_for_shadow_map_pass();
		void bind_for_skybox_pass();
		void bind_for_final_pass();
	
		U32 m_fbo;
		U32 m_textures[FRAME_NUM_TEXTURES];
		U32 m_depth_texture;
		U32 m_shadow_texture; //Holds shadow mapping informations
		U32 m_final_texture; //Render here the final image, and then send it to screen 

	};
}