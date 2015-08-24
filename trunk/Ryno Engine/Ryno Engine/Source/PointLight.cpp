#include "PointLight.h"

namespace Ryno{


	PointLight::PointLight() {
	}
	


	void PointLight::send_uniforms(Camera3D* camera){

		glUniform4f(locations.position, position.x, position.y, -position.z,attenuation);
		glUniform4f(locations.diffuse, diffuse_color.r/256.0f ,diffuse_color.g/256.0f,diffuse_color.b/256.0f,diffuse_intensity);
		glUniform4f(locations.specular, specular_color.r/256.0f, specular_color.g/256.0f, specular_color.b/256.0f,specular_intensity);

	}


	void PointLight::get_uniforms_locations()
	{
		locations.position = program->getUniformLocation("point_light.position_and_attenuation");
		locations.diffuse = program->getUniformLocation("point_light.diffuse");
		locations.specular = program->getUniformLocation("point_light.specular");
	}

	void PointLight::calculate_max_radius()
	{
		//Get the max radius as the max between the specular and the diffuse one
		F32 max_diffuse = fmax(fmax(diffuse_color.r, diffuse_color.g), diffuse_color.b);
		F32 max_specular = fmax(fmax(specular_color.r, specular_color.g), specular_color.b);

		

		//Compare before the sqrt, because it's expensive
		F32 max_value = fmax(diffuse_intensity* max_diffuse / attenuation, specular_intensity*max_specular / attenuation);


		max_radius = sqrt(max_value);
	}
}