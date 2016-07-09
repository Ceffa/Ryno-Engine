#pragma once

#include "Lights/AbstractLight.h"
#include <list>

namespace RynoEngine{
	

	class SpotLight : public AbstractLight{
	public:
	
		SpotLight() { blur = 0; shadow_strength = .75f; }
		~SpotLight(){}
		SpotLight* clone() override { return new SpotLight(*this); }

	
		void set_rotation(F32 _pitch, F32 _yaw, F32 _roll);
		void set_rotation(glm::vec3 euler);


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
		U32 blur;

	};

}