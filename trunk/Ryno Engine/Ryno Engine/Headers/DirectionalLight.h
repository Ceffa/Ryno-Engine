#pragma once

#include "AbstractLight.h"

namespace Ryno{
	
	struct Direction{
		void set_direction(F32 _p, F32 _y, F32 _r){
			pitch = _p;
			yaw = _y;
			roll = _r;
		}
		F32 pitch;
		F32 yaw;
		F32 roll;
	};

	class DirectionalLight : public AbstractLight{
	public:
		DirectionalLight(){}
		DirectionalLight(F32 p, F32 y, F32 r);
		~DirectionalLight(){}

		glm::vec3 move_to_view_space(Camera3D* camera) override;
		
		void send_uniforms(Camera3D* camera) override;

		void set_direction(F32 p, F32 y, F32 r);

		void set_ambient_color(U8 r, U8 g, U8 b, U8 a);
		glm::vec4 get_ambient_color();

		//Since i only use one directional light, i calculate here the ambient light
		ColorRGBA ambient_color;
		F32 ambient_intensity;
		Direction direction;
	
	};

}