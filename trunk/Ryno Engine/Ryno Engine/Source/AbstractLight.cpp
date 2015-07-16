#include "AbstractLight.h"

namespace Ryno{

	void AbstractLight::set_diffuse_color(U8 r, U8 g, U8 b){
		diffuse_color.set_color(r, g, b);
	}
	void AbstractLight::set_specular_color(U8 r, U8 g, U8 b){
		specular_color.set_color(r, g, b);
	}

	glm::vec3 AbstractLight::get_diffuse_color(){
		return diffuse_color.to_vec3();
	}
	glm::vec3 AbstractLight::get_specular_color(){
		return specular_color.to_vec3();
	}
	

}