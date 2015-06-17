#include "Camera3D.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>

namespace Ryno{

	Camera3D::Camera3D(U32 w, U32 h) :width(w), height(h)
	{
	
	}
	Camera3D::~Camera3D(){

	}

	void Camera3D::generate_camera_matrix(){
		//scale
		camera_matrix = glm::scale(glm::mat4(1.0), glm::vec3(scale/2.0f));
		//rotate
		camera_matrix = glm::rotate(camera_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		camera_matrix = glm::rotate(camera_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		camera_matrix = glm::rotate(camera_matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//translate
		camera_matrix = glm::translate(camera_matrix, glm::vec3(position.x-(width/2), position.y -(height/2), position.z));
	
		camera_matrix = glm::perspective(
			45.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
			800.0f / 600.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
			0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
			1000.0f       // Far clipping plane. Keep as little as possible.
			) * camera_matrix;
	
	}

}