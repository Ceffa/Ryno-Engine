#include "FBOs/FBO_Shadow.h"
#include "Log.h"
#include "Global.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <Glm/gtx/transform.hpp>
#define NINETY_DEGREES 1.57079632679489661923

namespace Ryno {

	


	FBO_Shadow::FBO_Shadow(U32 width, U32 height){
		init(width, height);
	}


	void FBO_Shadow::init(U32 width, U32 height){

		// Create and bind the FBO
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	
		// Create the directional textures
		glGenTextures(1, &m_spot_texture);
		glBindTexture(GL_TEXTURE_2D, m_spot_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, spot_resolution, spot_resolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_spot_texture, 0);

		// Create the directional textures
		glGenTextures(1, &m_directional_texture);
		glBindTexture(GL_TEXTURE_2D, m_directional_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, directional_resolution, directional_resolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_directional_texture, 0);

		
		// Create the cube map
		glGenTextures(1, &m_point_cube);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_point_cube);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);


		for (U8 i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, point_resolution, point_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	
	
		//Check if ok
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FB error." <<  std::endl;
	
		}

	


	}

	void FBO_Shadow::start_frame()
	{
		//Binds the shadow framebuffer, and then clear the buffer
		bind_fbo();

		glClear(GL_DEPTH_BUFFER_BIT);
	}


	

	void FBO_Shadow::bind_for_directional_shadow_pass(){

		bind_fbo();
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_directional_texture, 0);

		//Draw only depth
		glDrawBuffer(GL_NONE);
		

		}


	void FBO_Shadow::bind_for_point_shadow_pass()
	{
		bind_fbo();
		//Bind all cubemap, the geometry shader will take care of the faces
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_point_cube, 0);
		glDrawBuffer(GL_NONE);


	}

	


	void FBO_Shadow::bind_for_spot_shadow_pass()
	{
		bind_fbo();
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_spot_texture, 0);

		//Draw only depth
		glDrawBuffer(GL_NONE);
	}
	
	

void FBO_Shadow::bind_fbo()
	{
		I32 old_fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
		if (old_fbo != m_fbo)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	}

}
	
