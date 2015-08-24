#pragma once

#include "AbstractLight.h"

namespace Ryno{
	

	class SpotLight : public AbstractLight{
	public:
		SpotLight();
		~SpotLight(){}

		void send_uniforms(Camera3D* camera) override;
		void get_uniforms_locations() override;

		void set_position(F32 x, F32 y, F32 z){
			position.x = x;
			position.y = y;
			position.z = z;
		};
		void set_direction(F32 p, F32 y, F32 r){
			direction.x = p;
			direction.y = y;
			direction.z = r;
		};

		void calculate_max_radius();
		
		F32 max_radius;
		glm::vec3 position;
		F32 attenuation; //exp
		glm::vec3 direction;
		F32 cutoff; //Angle of the cone

		struct location{
			I32 position;
			I32 direction;
			I32 diffuse;
			I32 specular;
		} locations;
		

	};

}