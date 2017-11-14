#pragma once

#include <iostream>
#include "Global.h"
#include "Camera3D.h"
#include "Structures.h"
#include "Model.h"
#include <GLM\glm.hpp>
#include "Component.h"

namespace Ryno{
	
	
	class AbstractLight : public Component{
	public:
		void set_diffuse_color(U8 r, U8 g, U8 b);
		void set_specular_color(U8 r, U8 g, U8 b);

		bool active = true;

		glm::vec3 get_diffuse_color();
		glm::vec3 get_specular_color();

		bool is_unique() override { return false; }
		bool is_copyable() override { return true; }
		

		ColorRGBA diffuse_color, specular_color;
		F32 diffuse_intensity, specular_intensity;
		bool shadows = true;
		F32 shadow_strength;

	};

}