#pragma once

#include "Types.h"
#include "Camera3D.h"
#include "Structures.h"
#include <GLM\glm.hpp>

namespace Ryno{
	
	
	class LightPoint{
	public:
		LightPoint();
		LightPoint(F32 _x, F32 _y, F32 _z);
		~LightPoint(){}

		glm::vec3 get_view_space_position(Camera3D* camera);
		F32 calculate_max_radius();
		void set_position(F32 _x, F32 _y, F32 _z);

		F32 x;
		F32 y;
		F32 z;
		F32 attenuation; //exponential
		F32 intensity;
		ColorRGBA color;

	};

}