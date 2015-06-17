#pragma once
#include "Types.h"
#include <GLM/glm.hpp>


namespace Ryno{
	class Camera3D{
	public:
		Camera3D(U32 w, U32 h);
		~Camera3D();

		void generate_camera_matrix();

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		I32 width;
		I32 height;
		glm::mat4 camera_matrix;

		



	};
}