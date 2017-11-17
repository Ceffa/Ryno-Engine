#include "Lights/PointLight.h"

namespace Ryno{

	PointLight::PointLight()
	{
		shadow_strength = .75f;
		point_lights.insert(this);
	}

	PointLight::~PointLight()
	{
		point_lights.erase(this);
	}

	PointLight* PointLight::clone()
	{
		PointLight* temp = new PointLight(*this);
		point_lights.insert(temp);
		return temp;
	}

	std::set<PointLight*> PointLight::point_lights;

	void PointLight::calculate_max_radius()
	{
		//Get the max radius as the max between the specular and the diffuse one
		F32 max_diffuse = fmax(fmax(diffuse_color.r, diffuse_color.g), diffuse_color.b);
		F32 max_specular = fmax(fmax(specular_color.r, specular_color.g), specular_color.b);

		
		//Compare before the sqrt, because it's expensive
		F32 max_value = fmax(diffuse_intensity* max_diffuse, specular_intensity*max_specular);


		max_radius = std::pow(max_value-1, 1.0f / attenuation);
	}
}