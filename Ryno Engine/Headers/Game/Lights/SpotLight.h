#pragma once

#include "Lights/AbstractLight.h"
#include <list>

namespace Ryno{
	

	class SpotLight : public AbstractLight{
	public:
	
		SpotLight(){ model = nullptr; }
		~SpotLight(){}
		SpotLight(const SpotLight &copy)
		{
			*this = copy;
			if (copy.model)
				model = new Model(*copy.model);
			else model = nullptr;
		}
	
		void set_rotation(F32 _pitch, F32 _yaw, F32 _roll);


		void calculate_max_radius();
		
		//Light data
		F32 max_radius;
		F32 attenuation; //exp
		glm::quat rotation;

		F32 cutoff; //Angle of the cone

		//Shaders uniform locations
		struct location{
			I32 position;
			I32 direction;
			I32 diffuse;
			I32 specular;
		} locations;

		//If true, rotate along with game object transform. If not, only use its position
		bool absolute_movement = false;

	};

}