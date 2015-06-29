#include "LightPoint.h"

namespace Ryno{


	LightPoint::LightPoint(F32 _x, F32 _y, F32 _z) : x(_x), y(_y), z(_z){}
	

	glm::vec3 LightPoint::get_view_space_position(Camera3D* camera){
		glm::vec4 pos = camera->get_view_matrix()*glm::vec4(x, y, -z, 1);
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	void LightPoint::set_position(F32 _x, F32 _y, F32 _z){
		x = _x;
		y = _y;
		z = _z;
	}
}