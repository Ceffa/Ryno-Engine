#include "LightPoint.h"

namespace Ryno{


	LightPoint::LightPoint(F32 _x, F32 _y, F32 _z) : x(_x), y(_y), z(_z){
		attenuation = 0;
		
		intensity = 1;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.f = 255;
	}
	

	glm::vec3 LightPoint::get_view_space_position(Camera3D* camera){
		glm::vec4 pos = camera->get_V_matrix()*glm::vec4(x, y, -z, 1);
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	void LightPoint::set_position(F32 _x, F32 _y, F32 _z){
		x = _x;
		y = _y;
		z = _z;
	}

	F32 LightPoint::calculate_max_radius()
	{
		F32 MaxChannel = fmax(fmax(color.r, color.g), color.b);

		F32 ret = sqrt(MaxChannel * 255 / attenuation);
		return ret;
	}
}