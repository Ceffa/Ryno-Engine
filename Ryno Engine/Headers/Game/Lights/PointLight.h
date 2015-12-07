#pragma once

#include "Lights/AbstractLight.h"
#include <list>
namespace Ryno{
	

	class PointLight : public AbstractLight{
	public:

		static void* operator new(size_t size, MemoryLocation mem_loc){
			return Allocator::alloc(size, mem_loc);
		}

		//Constructors
		PointLight(){}
		PointLight(const PointLight *p){ *this = *p; }
		

	


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

	private:
		~PointLight();
		
	};

}