#include "FrameBuffer.h"
#include "Log.h"
#include <GL/glew.h>
#include <GL/gl.h>

namespace Ryno {

	FrameBuffer::FrameBuffer(U32 width, U32 height){
		init(width, height);
	}


	void FrameBuffer::init(U32 width, U32 height){

		// Create the FBO
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

		// Create the gbuffer textures
		glGenTextures(FRAME_NUM_TEXTURES, m_textures);
		glGenTextures(1, &m_depth_texture);

		for (U32 i = 0; i < FRAME_NUM_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
		}

		// depth
		glBindTexture(GL_TEXTURE_2D, m_depth_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,nullptr);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(FRAME_NUM_TEXTURES, DrawBuffers);

		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			Log::FatalError("FB error.");
	
		}

		// restore default FBO
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


	}

	void FrameBuffer::bind_for_reading(){
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	}
	void FrameBuffer::bind_for_writing(){
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	}
	void FrameBuffer::set_read_buffer(FRAME_TEXTURE_TYPE texture_type){
		glReadBuffer(GL_COLOR_ATTACHMENT0 + texture_type);
	}
}
	
