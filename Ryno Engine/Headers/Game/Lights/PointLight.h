#pragma once

#include "Lights/AbstractLight.h"
#include <list>
namespace Ryno{
	

	class PointLight : public AbstractLight{
	public:

		//Constructors
		PointLight(){}
		~PointLight(){}
		PointLight(const PointLight &copy)
		{
			*this = copy;
			model.copy(copy.model);
		}

	


		void calculate_max_radius();
		
		//Light data
		F32 max_radius;
		F32 attenuation; //exp

		//Light uniform locations
		struct location{
			I32 position;
			I32 diffuse;
			I32 specular;
		} locations;

	
	};

}