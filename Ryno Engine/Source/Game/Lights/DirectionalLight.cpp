#include "Lights/DirectionalLight.h"

namespace Ryno{


	DirectionalLight::DirectionalLight()
	{
		blur = 0; shadow_strength = .75f; dir_lights.insert(this);
	}

	DirectionalLight::~DirectionalLight()
	{
		dir_lights.erase(this);
	}

	DirectionalLight* DirectionalLight::clone()
	{

		DirectionalLight* temp = new DirectionalLight(*this);
		dir_lights.insert(temp);
		return temp;
	}

	std::set<DirectionalLight*> DirectionalLight::dir_lights;

	void DirectionalLight::set_rotation(F32 _pitch, F32 _yaw, F32 _roll)
	{
		set_rotation(glm::vec3(_pitch, _yaw, _roll));
	}
	void DirectionalLight::set_rotation(glm::vec3 euler)
	{
		rotation = glm::quat(euler*(F32)DEG_TO_RAD);
	}
	void DirectionalLight::set_ambient_color(U8 r, U8 g, U8 b){
		ambient_color.set_color(r, g, b);
	}

	glm::vec3 DirectionalLight::get_ambient_color(){
		return ambient_color.to_vec3();
	}
}