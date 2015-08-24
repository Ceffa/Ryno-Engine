#pragma once

#include "AbstractLight.h"

namespace Ryno{
	
	struct Direction{
		void set_direction(F32 _p, F32 _y, F32 _r){
			pitch = _p;
			yaw = _y;
			roll = _r;
		}
		glm::vec3 to_vec3(){ return glm::vec3(pitch, yaw, roll); }
		F32 pitch;
		F32 yaw;
		F32 roll;
	};

	class DirectionalLight : public AbstractLight{
	public:
		DirectionalLight();
		~DirectionalLight(){}

		glm::vec3 move_to_view_space(Camera3D* camera);
		
		void send_uniforms(Camera3D* camera) override;
		void get_uniforms_locations() override;

		void set_direction(F32 p, F32 y, F32 r);

		void set_ambient_color(U8 r, U8 g, U8 b);
		glm::vec3 get_ambient_color();

		//Since i only use one directional light, i calculate here the ambient light
		ColorRGB ambient_color;
		F32 ambient_intensity;
		Direction direction;

		struct location{
			I32 direction;
			I32 diffuse;
			I32 specular;
			I32 ambient;
		} locations;
	
	};

}