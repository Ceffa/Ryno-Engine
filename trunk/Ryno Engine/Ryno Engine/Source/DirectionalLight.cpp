#include "DirectionalLight.h"

namespace Ryno{


	DirectionalLight::DirectionalLight(){


	
	}
	





	void DirectionalLight::set_direction(F32 p, F32 y){
		
			
		pitch = -p * DEG_TO_RAD;
		yaw = y * DEG_TO_RAD - M_HALF_PI;


		direction = glm::normalize(glm::vec3(cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch)));
		
		
	}

	void DirectionalLight::set_ambient_color(U8 r, U8 g, U8 b){
		ambient_color.set_color(r, g, b);
	}

	glm::vec3 DirectionalLight::get_ambient_color(){
		return ambient_color.to_vec3();
	}
}