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
		
		//Convert position (or direction) accordingly.
		//Must be overriden
		virtual glm::vec3 move_to_view_space(Camera3D* camera) = 0;

		//Send to shader all the light data
		virtual void send_uniforms(Camera3D* camera) = 0;

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