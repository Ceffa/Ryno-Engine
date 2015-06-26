#pragma once

#include "Types.h"

namespace Ryno{
	class FrameBuffer
	{
	public:
		FrameBuffer(U32 width, U32 height);
		~FrameBuffer(){}
		enum FRAME_TEXTURE_TYPE{
			FRAME_TEXTURE_TYPE_POSITION=0,
			FRAME_TEXTURE_TYPE_DIFFUSE=1,
			FRAME_TEXTURE_TYPE_NORMAL=2,
			FRAME_TEXTURE_TYPE_TEXCOORD=3,
			FRAME_NUM_TEXTURES=4
		};

		void init(U32 width, U32 height);
		void bind_for_reading();
		void bind_for_writing();
		void set_read_buffer(FRAME_TEXTURE_TYPE texture_type);

		U32 m_fbo;
		U32 m_textures[FRAME_NUM_TEXTURES];
		U32 m_depth_texture;
		 
	};
}