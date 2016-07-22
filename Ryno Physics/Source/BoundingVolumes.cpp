#include "BoundingVolumes.h"

namespace Ryno {

	BoundingSphere::BoundingSphere(const BoundingSphere& a, const BoundingSphere& b)
	{
		V3 center_offset = b.center - a.center;
		F distance = glm::length2(center_offset);
		F radius_diff = b.radius - a.radius;

		//If encompassing 
		if (radius_diff * radius_diff >= distance) {
			if (a.radius > b.radius) {
				center = a.center;
				radius = a.radius;
			}
			else {
				center = b.center;
				radius = b.radius;
			}
		}
		else {

			distance = sqrt(distance);
			radius = (distance + a.radius + b.radius) / 2.0;
			
			//Lerp to get center
			center = a.center + center_offset * ((radius - a.radius) / distance);
		}
	}

	bool BoundingSphere::overlaps(const BoundingSphere* other) const
	{
		F center_dis = glm::length2(center - other->center);
		F radius_diff = radius + other->radius;
		return (center_dis < radius_diff * radius_diff);
	}

	F BoundingSphere::get_growth(const BoundingSphere& other) const
	{
		BoundingSphere temp_new(*this, other);

		// We return a value proportional to the change in surface
		// area of the sphere.
		//The sphere goes out of scope because it's just a temp value
		return temp_new.radius*temp_new.radius - radius*radius;

	}

}