#include "FBO_Shadow.h"
#include "Log.h"
#include "Global.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

namespace Ryno {

	


	FBO_Shadow::FBO_Shadow(U32 width, U32 height){
		init(width, height);
	}


	void FBO_Shadow::init(U32 width, U32 height){

		// Create and bind the FBO
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

		// Create the frame buffer textures
		glGenTextures(1, &m_shadow_texture);
		glBindTexture(GL_TEXTURE_2D, m_shadow_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_texture, 0);
		
		// Create the cube map
		glGenTextures(1, &m_shadow_cube);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadow_cube);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (U8 i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
		}
		
	
		//Check if ok
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			Log::FatalError("FB error.");
	
		}

	


	}

	void FBO_Shadow::start_frame()
	{
		//Binds the shadow framebuffer, and then clear the buffer
		bind_fbo();

		
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	

	void FBO_Shadow::bind_for_shadow_map_pass(){

		bind_fbo();

		//Draw only depth
		glDrawBuffer(GL_NONE);
		

		}

	void FBO_Shadow::bind_for_directional_light_pass(){

		//bind shadow map so it can be used in the lighting shader
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_shadow_texture);
	}
	void FBO_Shadow::bind_for_point_light_pass(){
		bind_fbo();
	}


	void FBO_Shadow::bind_face(GLenum cube_face)
	{	
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cube_face, m_shadow_cube, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		
	}

	void FBO_Shadow::bind_fbo()
	{
		I32 old_fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
		if (old_fbo != m_fbo)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	}

	


	 
}
	
