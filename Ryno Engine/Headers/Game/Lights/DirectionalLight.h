#pragma once

#include "Lights/AbstractLight.h"

namespace Ryno{
	
	

	class DirectionalLight : public AbstractLight{
	public:

	
		DirectionalLight(){}
		~DirectionalLight(){}
		DirectionalLight(const DirectionalLight *d){ *this = *d; }

		
	
		void set_direction(F32 p, F32 y);

		void set_ambient_color(U8 r, U8 g, U8 b);
		glm::vec3 get_ambient_color();

		//Since i only use one directional light, i calculate here the ambient light
		ColorRGB ambient_color;
		F32 ambient_intensity;
		glm::vec3 direction;
		F32 pitch, yaw;

		//uniform info
		struct location{
			I32 direction;
			I32 diffuse;
			I32 specular;
			I32 ambient;
		} locations;


	};

}