#pragma once
#include "Types.h"
#include <GLM/glm.hpp>


namespace Ryno{
	class Camera3D{
	public:
		Camera3D(U32 w, U32 h);
		~Camera3D();

		void generate_camera_matrix();
		void move_forward(F32 speed);
		void move_right(F32 speed);
		void move_back(F32 speed);
		void move_left(F32 speed);
		void rotate(glm::vec3 rot);
		void rotate(F32 x, F32 y, F32 z);

		
		I32 width;
		I32 height;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::mat4 camera_matrix;
		glm::mat4 perspective_matrix;
	private:
		

				



	};
}