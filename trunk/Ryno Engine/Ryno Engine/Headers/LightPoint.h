#pragma once

#include "Types.h"
#include "Camera3D.h"
#include <GLM\glm.hpp>

namespace Ryno{
	
	class LightPoint{
	public:
		LightPoint(){}
		LightPoint(F32 _x, F32 _y, F32 _z);
		~LightPoint(){}

		glm::vec3 get_view_space_position(Camera3D* camera);
		void set_position(F32 _x, F32 _y, F32 _z);
	private:
		F32 x;
		F32 y;
		F32 z;

	};

}