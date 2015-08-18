#include "FBO_Deferred.h"
#include "Log.h"
#include "Global.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

namespace Ryno {

	FBO_Deferred::FBO_Deferred(U32 width, U32 height){
		init(width, height);
	}


	void FBO_Deferred::init(U32 width, U32 height){

		// Create and bind the FBO
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

		// Create the frame buffer textures
		glGenTextures(FRAME_NUM_TEXTURES, m_textures);
		glGenTextures(1, &m_depth_texture);
		glGenTextures(1, &m_final_texture);

		
		//bind g color texture
		glBindTexture(GL_TEXTURE_2D, m_textures[FRAME_TEXTURE_TYPE_DIFFUSE]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[FRAME_TEXTURE_TYPE_DIFFUSE], 0);

		//bind g normal texture
		glBindTexture(GL_TEXTURE_2D, m_textures[FRAME_TEXTURE_TYPE_NORMAL]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_textures[FRAME_TEXTURE_TYPE_NORMAL], 0);

		//bind g depth texture
		glBindTexture(GL_TEXTURE_2D, m_textures[FRAME_TEXTURE_TYPE_DEPTH]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_textures[FRAME_TEXTURE_TYPE_DEPTH], 0);

		//NON-DEFERRED TEXTURES

		//Bind depth texture (with 8 bits for stencil)
		glBindTexture(GL_TEXTURE_2D, m_depth_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

		
		//Bind final texture
		glBindTexture(GL_TEXTURE_2D, m_final_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_final_texture, 0);

		//Check if ok
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			Log::FatalError("FB error.");
	
		}

	


	}

	void FBO_Deferred::start_frame()
	{
		//Binds the custom framebuffer, and then clear the previous final_texture
		bind_fbo();

		glDrawBuffer(GL_COLOR_ATTACHMENT4);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void FBO_Deferred::bind_for_geometry_pass()
	{
		//Binds custom buffer, specify draw buffers, and set them to draw
		bind_fbo();

		GLenum DrawBuffers[] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2
		};

		glDrawBuffers(FRAME_NUM_TEXTURES, DrawBuffers);
	}

	void FBO_Deferred::bind_for_stencil_pass()
	{
		bind_fbo();
	
		//Disable all draw buffers, cause it just wants to get depth and stencil.
		//Without this, the drawing would override geometry pass (because the fbo is the same)
		glDrawBuffer(GL_NONE);
	}


	void FBO_Deferred::bind_for_light_pass()
	{
		bind_fbo();

		//Draw in the final_texture of fbo, not yet in the screen buffer
		glDrawBuffer(GL_COLOR_ATTACHMENT4);

		//Bind the textures correspondent to the buffers of the gbuffer.
		//The light shader needs textures position (like GL_TEXTURE0), not texture ids.
		//So we need to bind the id to the location.

		for (U8 i = 0; i < FRAME_NUM_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		}

	/*	glActiveTexture(GL_TEXTURE0 + FRAME_NUM_TEXTURES);
		glBindTexture(GL_TEXTURE_2D, m_depth_texture);*/
		
	}


	void FBO_Deferred::bind_for_skybox_pass(){

		glDrawBuffer(GL_NONE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textures[2]);


	}

	void FBO_Deferred::bind_for_final_pass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	

		/*glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			 WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, 2 * WINDOW_WIDTH / 3, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			2*WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, WINDOW_WIDTH , WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, 0, WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
	
		glReadBuffer(GL_COLOR_ATTACHMENT4);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			2*WINDOW_WIDTH / 3, 0,  WINDOW_WIDTH , WINDOW_HEIGHT / 2, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/
	
		glReadBuffer(GL_COLOR_ATTACHMENT4);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0,0, WINDOW_WIDTH/2 , WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	}


	 
	void FBO_Deferred::bind_fbo()
	{
		I32 old_fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
		if (old_fbo!=m_fbo)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	}

}
	
