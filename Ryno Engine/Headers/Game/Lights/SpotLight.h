#pragma once

#include "Lights/AbstractLight.h"
#include <set>

namespace Ryno{
	

	class SpotLight : public AbstractLight{
	public:
	
		SpotLight();
		~SpotLight();
		SpotLight* clone() override;

		static std::set<SpotLight*> spot_lights;
	
		void set_rotation(F32 _pitch, F32 _yaw, F32 _roll);
		void set_rotation(glm::vec3 euler);


		void calculate_max_radius();
		
		//Light data
		F32 max_radius;
		F32 attenuation = 2; //exp
		glm::quat rotation;

		F32 inner_angle; 
		F32 outer_angle; 

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