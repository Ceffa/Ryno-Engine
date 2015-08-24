#include "SpotLight.h"

namespace Ryno{


	SpotLight::SpotLight() {
	
		direction = glm::vec3(0, 1, 0);
		cutoff = .1f;
		attenuation = 0;
	}
	

	
	void SpotLight::send_uniforms(Camera3D* camera){

		glm::vec3 norm_dir = glm::normalize(direction);
		glUniform4f(locations.position, position.x, position.y, -position.z, attenuation);
		glUniform4f(locations.direction, norm_dir.x, norm_dir.y, -norm_dir.z , cutoff);
		glUniform4f(locations.diffuse, diffuse_color.r / 256.0f, diffuse_color.g / 256.0f, diffuse_color.b / 256.0f, diffuse_intensity);
		glUniform4f(locations.specular, specular_color.r/256.0f, specular_color.g/256.0f, specular_color.b/256.0f,specular_intensity);

	}


	void SpotLight::get_uniforms_locations()
	{
		locations.position = program->getUniformLocation("spot_light.position_and_attenuation");
		locations.direction = program->getUniformLocation("spot_light.direction_and_cutoff");
		locations.diffuse = program->getUniformLocation("spot_light.diffuse");
		locations.specular = program->getUniformLocation("spot_light.specular");

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