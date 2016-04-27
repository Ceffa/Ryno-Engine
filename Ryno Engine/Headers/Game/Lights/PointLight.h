#pragma once

#include "Lights/AbstractLight.h"
#include <list>
namespace Ryno{
	

	class PointLight : public AbstractLight{
	public:

		//Constructors
		PointLight(){ model = nullptr; }
		~PointLight(){}
		PointLight(const PointLight &copy)
		{
			*this = copy;
			if (copy.model)
				model = new Model(*copy.model);
			else model = nullptr;
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