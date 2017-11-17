#include "FBOs/FBO_Shadow.h"
#include "Log.h"
#include "Global.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <Glm/gtx/transform.hpp>
#define NINETY_DEGREES 1.57079632679489661923

namespace Ryno {




	FBO_Shadow::FBO_Shadow(U32 width, U32 height) {
		init(width, height);
	}


	void FBO_Shadow::init(U32 width, U32 height) {

		// Create and bind the FBO
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);


		// Create the directional textures
		glGenTextures(1, &m_spot_texture);
		glBindTexture(GL_TEXTURE_2D, m_spot_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, spot_resolution, spot_resolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_spot_texture, 0);

		// Create the directional textures
		glGenTextures(1, &m_directional_texture);
		glBindTexture(GL_TEXTURE_2D, m_directional_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, directional_resolution, directional_resolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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


		create_jitter(0,4);
		create_jitter(1, 6);
		create_jitter(2, 8);




		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);



		//Check if ok
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FB error." << std::endl;

		}
	}

#define SAMPLES_U 8
#define SAMPLES_V 8

	void FBO_Shadow::create_jitter(U8 index, U8 SIZE)
	{
		// Create the cube map
		glGenTextures(1, &m_jitter[index]);
		glBindTexture(GL_TEXTURE_3D, m_jitter[index]);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		//Each column holds all the offsets. There are size * size columns.
		//The division by two is because in a vec4 are contained two pairs of jitters
		I8 * data = new I8[SIZE * SIZE * SAMPLES_U * SAMPLES_V * 4 / 2];

		for (U8 i = 0; i < SIZE; i++) {
			for (U8 j = 0; j < SIZE; j++) {
				for (U8 k = 0; k < SAMPLES_U*SAMPLES_V / 2; k++) {

					I32 x, y;
					glm::vec4 v;

					//Get two dimensional positions
					x = k % (SAMPLES_U / 2);
					y = (SAMPLES_V - 1) - k / (SAMPLES_U / 2);

					// generate points on a regular samples_u x samples_v rectangular grid.
					v[0] = (F32)(x * 2 + 0.5f) / SAMPLES_U;
					v[1] = (F32)(y + 0.5f) / SAMPLES_V;
					v[2] = (F32)(x * 2 + 1 + 0.5f) / SAMPLES_U;
					v[3] = v[1];

					// jitter position   
					v[0] += ((F32)rand() * 2 / RAND_MAX - 1) * (0.5f / SAMPLES_U);
					v[1] += ((F32)rand() * 2 / RAND_MAX - 1) * (0.5f / SAMPLES_V);
					v[2] += ((F32)rand() * 2 / RAND_MAX - 1) * (0.5f / SAMPLES_U);
					v[3] += ((F32)rand() * 2 / RAND_MAX - 1) * (0.5f / SAMPLES_V);

					// warp to disk   
					glm::vec4 d;
					d[0] = sqrtf(v[1]) * cosf(2 * 3.1415926f * v[0]);
					d[1] = sqrtf(v[1]) * sinf(2 * 3.1415926f * v[0]);
					d[2] = sqrtf(v[3]) * cosf(2 * 3.1415926f * v[2]);
					d[3] = sqrtf(v[3]) * sinf(2 * 3.1415926f * v[2]);

					data[(k * SIZE * SIZE + j * SIZE + i) * 4 + 0] = (I8)(d[0] * 127);
					data[(k * SIZE * SIZE + j * SIZE + i) * 4 + 1] = (I8)(d[1] * 127);
					data[(k * SIZE * SIZE + j * SIZE + i) * 4 + 2] = (I8)(d[2] * 127);
					data[(k * SIZE * SIZE + j * SIZE + i) * 4 + 3] = (I8)(d[3] * 127);
				}
			}
		}

		glTexImage3D(GL_TEXTURE_3D, 0, GL_SIGNED_RGBA_NV, SIZE, SIZE, SAMPLES_U * SAMPLES_V / 2, 0, GL_RGBA, GL_BYTE, data);


		delete[] data;
	}

	void FBO_Shadow::start_frame()
	{
		//Binds the shadow framebuffer, and then clear the buffer
		bind_fbo();

		glClear(GL_DEPTH_BUFFER_BIT);
	}




	void FBO_Shadow::bind_for_directional_shadow_pass() {

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

