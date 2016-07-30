#pragma once
#include "Core.h"
#include "Narrow.h"

namespace Ryno {

	class InertiaTensorGenerator {
	public:
		
		glm::mat3 get_sphere_tensor(const CollisionSphere& s, F mass) const {
			F v = s.radius * s.radius * mass * 2.0f / 5.0f;
			return glm::mat3(
				v, 0, 0,
				0, v, 0,
				0, 0, v
				);
		}
		
		//Sphere with mass on the surface
		glm::mat3 get_shell_tensor(const CollisionSphere& s, F mass) const {
			F v = s.radius * s.radius * mass * 2.0f / 3.0f;
			return glm::mat3(
				v, 0, 0,
				0, v, 0,
				0, 0, v
				);
		}

		glm::mat3 get_cube_tensor(const CollisionBox& s, F mass) const {
			F f = mass / 12.0f;
			F x = s.half_size.x * s.half_size.x * 4;
			F y = s.half_size.y * s.half_size.y * 4;
			F z = s.half_size.z * s.half_size.z * 4;

			return glm::mat3(
				(y+z) * f, 0, 0,
				0, (x + z) * f, 0,
				0, 0, (y + x) * f
				);
		}

	};

}