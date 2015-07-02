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

		void set_diffuse_color(U8 r, U8 g, U8 b, U8 a);
		void set_specular_color(U8 r, U8 g, U8 b, U8 a);

		glm::vec4 get_diffuse_color();
		glm::vec4 get_specular_color();
		
		ColorRGBA diffuse_color, specular_color;
		F32 diffuse_intensity, specular_intensity;

		//Light program
		GLSLProgram* program;

	};

}