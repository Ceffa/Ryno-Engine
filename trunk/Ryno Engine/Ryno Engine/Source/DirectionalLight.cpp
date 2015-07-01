#include "DirectionalLight.h"

namespace Ryno{


	DirectionalLight::DirectionalLight(F32 p, F32 y, F32 r) : pitch(p), yaw(y), roll(r){}
	

	glm::vec3 DirectionalLight::get_view_space_direction(Camera3D* camera){
		roll -= roll;
		glm::vec4 rotated_dir = glm::transpose(glm::inverse(camera->get_view_matrix()))*glm::vec4(pitch,yaw,roll,0);
		return glm::vec3(rotated_dir.x, rotated_dir.y, rotated_dir.z);
	}

	void DirectionalLight::set_direction(F32 p, F32 y, F32 r){
		pitch = p;
		yaw = y;
		roll = r;
	}
}