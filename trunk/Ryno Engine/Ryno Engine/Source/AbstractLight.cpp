#include "AbstractLight.h"

namespace Ryno{

	void AbstractLight::set_diffuse_color(U8 r, U8 g, U8 b, U8 a){
		diffuse_color.set_color(r, g, b, a);
	}
	void AbstractLight::set_specular_color(U8 r, U8 g, U8 b, U8 a){
		specular_color.set_color(r, g, b, a);
	}

	glm::vec4 AbstractLight::get_diffuse_color(){
		return diffuse_color.to_vec4();
	}
	glm::vec4 AbstractLight::get_specular_color(){
		return specular_color.to_vec4();
	}
	

}