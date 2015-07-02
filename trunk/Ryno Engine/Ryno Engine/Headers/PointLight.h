#pragma once

#include "AbstractLight.h"

namespace Ryno{
	

	class PointLight : public AbstractLight{
	public:
		PointLight(F32 x, F32 y, F32 z);
		~PointLight(){}

		glm::vec3 move_to_view_space(Camera3D* camera) override;
		F32 calculate_max_radius();
		

		glm::vec3 position;
		F32 attenuation; //exp
		

	};

}