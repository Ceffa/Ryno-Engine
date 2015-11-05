#pragma once

#include "Global.h"
#include "GLSLProgram.h"
namespace Ryno{
	class FBO_Deferred
	{
	public:
		FBO_Deferred(U32 width, U32 height);
		~FBO_Deferred(){}
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
		void bind_for_skybox_pass();
		void bind_for_final_rendering_pass();
		void bind_for_GUI_pass();

	
		U32 m_fbo;
		U32 m_textures[FRAME_NUM_TEXTURES];
		U32 m_depth_texture;
		U32 m_final_texture; //Render here the final image, and then send it to screen 

	private:
		void bind_fbo();

	};
}