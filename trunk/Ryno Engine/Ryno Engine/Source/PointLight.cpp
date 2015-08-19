#include "PointLight.h"

namespace Ryno{


	PointLight::PointLight(F32 x, F32 y, F32 z) {
		position = glm::vec3(x, y, z);
		attenuation = 0;
	}
	

	glm::vec3 PointLight::move_to_view_space(Camera3D* camera){
	
		return glm::vec3(camera->get_V_matrix()*
			glm::vec4(position.x, position.y, -position.z, 1));

	}

	void PointLight::send_uniforms(Camera3D* camera){

		glm::vec3 view_space_pos = move_to_view_space(camera);

		glUniform4f(program->getUniformLocation("point_light.position_and_attenuation"), view_space_pos.x, view_space_pos.y, view_space_pos.z,attenuation);
		glUniform4f(program->getUniformLocation("point_light.diffuse"), diffuse_color.r/256.0f ,diffuse_color.g/256.0f,diffuse_color.b/256.0f,diffuse_intensity);
		glUniform4f(program->getUniformLocation("point_light.specular"), specular_color.r/256.0f, specular_color.g/256.0f, specular_color.b/256.0f,specular_intensity);

	}


	F32 PointLight::calculate_max_radius()
	{
		//Get the max radius as the max between the specular and the diffuse one
		F32 max_diffuse = fmax(fmax(diffuse_color.r, diffuse_color.g), diffuse_color.b);
		F32 max_specular = fmax(fmax(specular_color.r, specular_color.g), specular_color.b);

		

		//Compare before the sqrt, because it's expensive
		F32 max_value = fmax(diffuse_intensity* max_diffuse / attenuation, specular_intensity*max_specular / attenuation);


		return sqrt(max_value);
	}
}