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


		//Bind depth texture (with 8 bits for stencil)
		glBindTexture(GL_TEXTURE_2D, m_shadow_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_texture, 0);

		
	
		//Check if ok
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			Log::FatalError("FB error.");
	
		}

	


	}

	void FBO_Shadow::start_frame()
	{
		//Binds the shadow framebuffer, and then clear the buffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

		
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	

	void FBO_Shadow::bind_for_shadow_map_pass(){

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
		//Draw only depth
		glDrawBuffer(GL_NONE);
		

		}

	void FBO_Shadow::bind_for_light_pass(){
		//bind shadow map so it can be used in the lighting shader
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_shadow_texture);
	}

	


	 
}
	
