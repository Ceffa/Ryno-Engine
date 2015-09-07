#pragma once

#include "AbstractLight.h"
#include <list>
namespace Ryno{
	

	class PointLight : public AbstractLight{
	public:

		//Constructors
		PointLight();
		PointLight(const PointLight *p);
		~PointLight();

	
		void set_position(F32 x, F32 y, F32 z);

		void calculate_max_radius();
		
		//Light data
		F32 max_radius;
		glm::vec3 position;
		F32 attenuation; //exp

		//Light uniform locations
		struct location{
			I32 position;
			I32 diffuse;
			I32 specular;
		} locations;
		
		//Point lights global list
		static std::list<PointLight*> point_lights;
		

	};

}