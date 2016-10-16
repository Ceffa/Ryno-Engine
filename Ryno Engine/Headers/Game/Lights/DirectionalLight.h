#pragma once

#include "Lights/AbstractLight.h"
#include <set>
namespace Ryno{
	
	

	class DirectionalLight : public AbstractLight{
	public:

	
		DirectionalLight();
		~DirectionalLight();

		DirectionalLight* clone() override;

		static std::set<DirectionalLight*> dir_lights;

		

		void set_rotation(F32 _pitch, F32 _yaw, F32 _roll);
		void set_rotation(glm::vec3 euler);


		void set_ambient_color(U8 r, U8 g, U8 b);
		glm::vec3 get_ambient_color();

		//Since i only use one directional light, i calculate here the ambient light
		ColorRGBA ambient_color;
		F32 ambient_intensity;
		glm::vec3 direction;
		
		glm::quat rotation;

		//uniform info
		struct location{
			I32 direction;
			I32 diffuse;
			I32 specular;
			I32 ambient;
		} locations;

		//If true, rotate along with game object transform. If not, only use its position
		bool absolute_movement = true;
		U32 blur;

	};

}