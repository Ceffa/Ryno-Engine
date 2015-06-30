#pragma once

#include "Types.h"
#include "Structures.h"
#include "Camera3D.h"
#include <GLM\glm.hpp>

namespace Ryno{
	
	class LightDirectional{
	public:
		LightDirectional(){}
		LightDirectional(F32 p, F32 y, F32 r);
		~LightDirectional(){}

		glm::vec3 get_view_space_direction(Camera3D* camera);
		void set_direction(F32 p, F32 y, F32 r);
		
		F32 intensity;
		ColorRGBA color;

	private:
		F32 pitch;
		F32 yaw;
		F32 roll;
	};

}