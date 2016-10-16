#include "Lights/SpotLight.h"
#include "Log.h"

namespace Ryno{

	SpotLight::SpotLight()
	{
		blur = 0; shadow_strength = .75f; spot_lights.insert(this);
	}

	SpotLight::~SpotLight()
	{
		spot_lights.erase(this);
	}

	SpotLight* SpotLight::clone()
	{
		SpotLight* temp = new SpotLight(*this);
		spot_lights.insert(temp);
		return temp;
	}

	std::set<SpotLight*> SpotLight::spot_lights;

	void SpotLight::set_rotation(F32 _pitch, F32 _yaw, F32 _roll)
	{		
		set_rotation(glm::vec3(_pitch , _yaw, _roll));
	}
	void SpotLight::set_rotation(glm::vec3 euler)
	{
		rotation = glm::quat(euler*(F32)DEG_TO_RAD);
	}

	void SpotLight::calculate_max_radius()
	{
		//Get the max radius as the max between the specular and the diffuse one
		F32 max_diffuse = fmax(fmax(diffuse_color.r, diffuse_color.g), diffuse_color.b);
		F32 max_specular = fmax(fmax(specular_color.r, specular_color.g), specular_color.b);

		

		//Compare before the sqrt, because it's expensive
		F32 max_value = fmax(diffuse_intensity* max_diffuse / attenuation, specular_intensity*max_specular / attenuation);


		max_radius = sqrt(max_value);
	}
}