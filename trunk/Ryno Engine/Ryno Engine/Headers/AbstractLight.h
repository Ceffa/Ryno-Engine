#pragma once

#include <iostream>
#include "Types.h"
#include "Camera3D.h"
#include "Structures.h"
#include "GLSLProgram.h"
#include <GLM\glm.hpp>

namespace Ryno{
	
	
	class AbstractLight{
	public:


		//Send to shader all the light data
		virtual void send_uniforms(Camera3D* camera) = 0;
		virtual void get_uniforms_locations() = 0;

		void set_program(GLSLProgram* _program);
		void set_diffuse_color(U8 r, U8 g, U8 b);
		void set_specular_color(U8 r, U8 g, U8 b);


		glm::vec3 get_diffuse_color();
		glm::vec3 get_specular_color();
		
		ColorRGB diffuse_color, specular_color;
		F32 diffuse_intensity, specular_intensity;

	
		//Light program
		GLSLProgram* program;

	};

}