#include "Camera3D.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <iostream>

#define M_PI 3.14159265358979323846
#define M_PI_2 6.28318530717958647692
namespace Ryno{

	Camera3D::Camera3D(U32 w, U32 h) :width(w), height(h)
	{
		perspective_matrix = glm::perspective(
			45.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
			width / (F32)height, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
			0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
			1000.0f       // Far clipping plane. Keep as little as possible.
			);
	
	}
	Camera3D::~Camera3D(){

	}

	void Camera3D::generate_camera_matrix(){
		//scale
		//camera_matrix = glm::scale(glm::mat4(1.0), scale);
		//rotate
		camera_matrix = glm::rotate(glm::mat4(1.0), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		camera_matrix = glm::rotate(camera_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		camera_matrix = glm::rotate(camera_matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//translate
		camera_matrix = perspective_matrix * glm::translate(camera_matrix, glm::vec3(-position.x, -position.y, position.z));
	
		
	}
	

	void Camera3D::move_forward(F32 speed){
		
		position += speed*glm::vec3(sin(rotation.y), -sin(rotation.x), cos(rotation.y));

	}
	void Camera3D::move_right(F32 speed){
		position += speed*glm::vec3(cos(rotation.y), -sin(rotation.x), -sin(rotation.y));

	}
	void Camera3D::move_back(F32 speed){
		move_forward(-speed);
	}
	void Camera3D::move_left(F32 speed){
		move_right(-speed);
	}
	void Camera3D::rotate(glm::vec3 rot){
		rotation+= rot;
		if (rotation.x > M_PI_2 || rotation.x <0) rotation.x -= ((I32)(rotation.x /M_PI_2))*M_PI_2;
		if (rotation.y > M_PI_2 || rotation.y <0) rotation.y -= ((I32)(rotation.y / M_PI_2))*M_PI_2;
		if (rotation.z > M_PI_2 || rotation.z <0) rotation.z -= ((I32)(rotation.z / M_PI_2))*M_PI_2;
	}
	void Camera3D::rotate(F32 x, F32 y, F32 z){
		rotate(glm::vec3(x, y, z));
	}

}