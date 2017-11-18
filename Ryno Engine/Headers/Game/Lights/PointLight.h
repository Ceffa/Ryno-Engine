#pragma once

#include "Lights/AbstractLight.h"
#include <set>
namespace Ryno{
	

	class PointLight : public AbstractLight{
	public:

		//Constructors
		PointLight();
		~PointLight();
		PointLight* clone() override;

		static std::set<PointLight*> point_lights;


		void calculate_max_radius();
		
		//Light data
		F32 max_radius;
		F32 attenuation = 2; //exp

	};

}