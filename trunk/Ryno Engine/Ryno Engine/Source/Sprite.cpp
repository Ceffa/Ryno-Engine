#include "Sprite.h"
#include "Global.h"

namespace Ryno{

	Sprite::Sprite(){
		
	}
	Sprite::Sprite(const Sprite *s)
	{
		*this = *s;
	}

	void Sprite::generate_model_matrix()
	{
		F32 deg_angle = -angle * DEG_TO_RAD;
		F32 sn = sin(deg_angle);
		F32 cs = cos(deg_angle);
		F32 new_x = (position.x - 0.5) * 2;
		F32 new_y = (position.y - 0.5) * 2;
		
		
		model_matrix = glm::mat3(
			glm::vec3(scale.x * cs, scale.x * sn,0),
			glm::vec3(-scale.y * sn, scale.y * cs, 0),
			glm::vec3(new_x, new_y, 1)
			);
	}

}

