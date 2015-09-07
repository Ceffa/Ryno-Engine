#pragma once

#include "AbstractLight.h"
#include <list>

namespace Ryno{
	

	class SpotLight : public AbstractLight{
	public:
		SpotLight();
		SpotLight(const SpotLight *s);
		~SpotLight();

	

		void set_position(F32 x, F32 y, F32 z);
		void set_direction(F32 _pitch, F32 _yaw);


		void calculate_max_radius();
		
		//Light data
		F32 max_radius;
		glm::vec3 position;
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

		//static list of spot lights
		static std::list<SpotLight*> spot_lights;
		

	};

}