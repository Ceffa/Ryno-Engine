#include "FrameBuffer.h"
#include "Log.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

namespace Ryno {

	FrameBuffer::FrameBuffer(U32 width, U32 height){
		init(width, height);
	}


	void FrameBuffer::init(U32 width, U32 height){

		// Create and bind the FBO
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

		// Create the frame buffer textures
		glGenTextures(FRAME_NUM_TEXTURES, m_textures);
		glGenTextures(1, &m_depth_texture);

		//We bind them just to initialize them and assign them to the p-buffers of the frame buffer
		for (U32 i = 0; i < FRAME_NUM_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
			//Disable filtering -> 1:1 with screen, so just use nearest interpolation
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);

		}

		//Same with the depth
		glBindTexture(GL_TEXTURE_2D, m_depth_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,nullptr);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

		//We tell the default wrtie buffer of the fragment shader, so it will output data in the p-buffers
		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(FRAME_NUM_TEXTURES, DrawBuffers);

		//Check if ok
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			Log::FatalError("FB error.");
	
		}

		// Restore default FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


	}

	void FrameBuffer::bind_for_reading(){
		//Bind default frame buffer for writing (thus automatically unbinding the custom frame buffer)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		//Bind the textures id to the opengl location, so the shader will read from them
		for (U32 i = 0; i < FRAME_NUM_TEXTURES; i++){
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		}
			
		
	}

	void FrameBuffer::send_uniforms(GLSLProgram* p){
		//Use at the beginning.
		//It will permanently tell the program to use the textures bound at location 0,1,2.
		//Do not confuse with textures ids.
		//Texture number 8 could be bound to location 0, here i'm interested in locations

		p->use();
		glUniform1i(p->getUniformLocation("m_pos"), 0);
		glUniform1i(p->getUniformLocation("m_col"), 1);
		glUniform1i(p->getUniformLocation("m_nor"), 2);
		p->unuse();


	}

	void FrameBuffer::bind_for_writing(){
		//Enable the custom framebuffer to drawing
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	}
	
}
	
