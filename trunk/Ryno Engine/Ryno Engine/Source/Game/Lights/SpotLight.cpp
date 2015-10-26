#include "Lights/SpotLight.h"
#include "Log.h"

namespace Ryno{

	void SpotLight::set_direction(F32 _pitch, F32 _yaw)
	{		
		pitch = _pitch * DEG_TO_RAD;
		yaw = _yaw * DEG_TO_RAD ;
		
		direction = glm::normalize(glm::vec3(cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch)));
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