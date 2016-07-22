#pragma once
#include "Core.h"


namespace Ryno {
	struct BoundingSphere {
		V3 center;
		F radius;

	public:
		BoundingSphere(const V3& _center, F _radius) : center(_center), radius(_radius) {}

		//Build a new bounding sphere that encompasses the other two
		BoundingSphere(const BoundingSphere& a, const BoundingSphere& b);

		bool overlaps(const BoundingSphere* other) const;

		//Return the growth in terms of surface
		F get_growth(const BoundingSphere& other) const;


	};

	struct BoundingBox {
		V3 center;
		V3 half_size;
	};

}