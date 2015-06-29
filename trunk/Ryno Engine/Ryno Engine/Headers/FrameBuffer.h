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
			FRAME_TEXTURE_TYPE_POSITION,
			FRAME_TEXTURE_TYPE_DIFFUSE,
			FRAME_TEXTURE_TYPE_NORMAL,
			FRAME_NUM_TEXTURES
		};

		void init(U32 width, U32 height);
		void bind_for_reading();
		void send_uniforms(GLSLProgram* p);
		void set_read_buffer(FRAME_TEXTURE_TYPE TextureType);
		void bind_for_writing();

		U32 m_fbo;
		U32 m_textures[FRAME_NUM_TEXTURES];
		U32 m_depth_texture;
		 
	};
}