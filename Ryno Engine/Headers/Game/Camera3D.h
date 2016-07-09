#pragma once
#include "Global.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "Structures.h"


namespace RynoEngine{
	class Camera3D{
	public:

		Camera3D(U32 w, U32 h);
		~Camera3D();

		void move_forward(F32 speed);
		void move_relative_to_view(F32 speed, glm::vec2 coords);
		void move_right(F32 speed);
		void move_back(F32 speed);
		void move_left(F32 speed);
		void rotate(F32 y, F32 p);
		glm::mat4& get_VP_matrix() { return VP_matrix; }
		glm::mat4& get_light_V_matrix() { return light_V_matrix; }
		glm::mat4& get_V_matrix() { return V_matrix; }
		glm::mat4& get_O_matrix() { return O_matrix; }
		glm::mat4& get_P_matrix() { return P_matrix; }
		void generate_matrices();

		static glm::mat4 generate_P_matrix(F32 angle, U32 width, U32 height, F32 near, F32 far);

		
		F32 movement_speed = 1.0f;
		F32 rotation_speed = 0.0005f;
		I32 width;
		I32 height;
		glm::vec4 position;
		F32 yaw, pitch;
		Texture skybox;
		ColorRGB background;
		bool have_skybox = false;

	private:
		glm::mat4 V_matrix;
		glm::mat4 VP_matrix;
		glm::mat4 light_V_matrix;
		glm::mat4 P_matrix;
		glm::mat4 O_matrix;

				



	};
}