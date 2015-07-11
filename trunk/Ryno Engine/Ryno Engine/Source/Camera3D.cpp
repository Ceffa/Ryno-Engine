#include "Camera3D.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <iostream>
#include <math.h>

#define M_HALF_PI 1.57079632679489661923
#define M_PI 3.14159265358979323846
#define M_PI_2 6.28318530717958647692
namespace Ryno{

	Camera3D::Camera3D(U32 w, U32 h) :width(w), height(h), yaw(0), pitch(0)
	{
		perspective_matrix = glm::perspective(
			45.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
			width / (F32)height, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
			1.0f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
			10000.0f       // Far clipping plane. Keep as little as possible.
			);
	
	}
	Camera3D::~Camera3D(){

	}

	void Camera3D::generate_camera_matrix(){
		
		//I ignore the scale.
		//I get the rotation from the pitch and yaw, and i make it faster with quaternions.
		//Finally i translate by the position 
		view_matrix = glm::translate(
			glm::toMat4(glm::quat(glm::vec3(pitch, 0, 0)) * glm::quat(glm::vec3(0, yaw, 0))),
			glm::vec3(-position.x, -position.y, position.z)
			); 
		camera_matrix = perspective_matrix * view_matrix;
						
	
	}
	

	void Camera3D::move_forward(F32 speed){
		position += speed* glm::vec4(sin(yaw), -sin(pitch), cos(yaw),0);
		
		

	}
	void Camera3D::move_right(F32 speed){
		position += speed* glm::vec4(cos(yaw),0,-sin(yaw),0);

	}
	void Camera3D::move_back(F32 speed){
		move_forward(-speed);
	}
	void Camera3D::move_left(F32 speed){
		move_right(-speed);
	}


	void Camera3D::rotate(F32 y, F32 p){
		yaw += y;
		if (yaw > M_PI_2 || yaw <0) yaw -= (F32)((I32)(yaw / M_PI_2))*M_PI_2;
		pitch += p;
		if (pitch > M_HALF_PI)
			pitch = M_HALF_PI;
		else if (pitch < -M_HALF_PI)
			pitch = -M_HALF_PI;


	
	}
	

}