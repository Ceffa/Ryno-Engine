#pragma once

#include "AbstractLight.h"

namespace Ryno{
	

	class PointLight : public AbstractLight{
	public:
		PointLight();
		~PointLight(){}

	
		void set_position(F32 x, F32 y, F32 z){
			position.x = x;
			position.y = y;
			position.z = z;
		};

		void calculate_max_radius();
		
		F32 max_radius;
		glm::vec3 position;
		F32 attenuation; //exp

		struct location{
			I32 position;
			I32 diffuse;
			I32 specular;
		} locations;
		

	};

}