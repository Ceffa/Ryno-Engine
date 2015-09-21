#pragma once

#include "Lights/AbstractLight.h"
#include <list>

namespace Ryno{
	

	class SpotLight : public AbstractLight{
	public:
		SpotLight(){}
		SpotLight(const SpotLight *s){ *this = *s; }
		~SpotLight(){}
	
		void set_direction(F32 _pitch, F32 _yaw);


		void calculate_max_radius();
		
		//Light data
		F32 max_radius;
		F32 attenuation; //exp
		glm::vec3 direction;
		F32 pitch, yaw;
		F32 cutoff; //Angle of the cone

		//Shaders uniform locations
		struct location{
			I32 position;
			I32 direction;
			I32 diffuse;
			I32 specular;
		} locations;


	};

}