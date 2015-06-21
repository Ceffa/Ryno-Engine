#pragma once
#include "Types.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>


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
		void rotate(F32 y, F32 p);
		glm::mat4 get_camera_matrix() const { return camera_matrix; }

		
		I32 width;
		I32 height;
		glm::vec4 position;
		glm::quat rotation;
		F32 yaw, pitch;
	
	private:
		
		glm::mat4 camera_matrix;
		glm::mat4 perspective_matrix;
				



	};
}