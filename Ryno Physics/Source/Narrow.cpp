#include "Narrow.h"
#include <GLM//gtc/matrix_access.hpp>
#include <iostream>

namespace Ryno {

	void CollisionData::clear()
	{
		for (auto c : contacts)
			delete c;
		contacts.clear();
	}

	U CollisionDetector::sphere_and_sphere(const CollisionSphere &one, const CollisionSphere &two, CollisionData& data)
	{
		if (data.max_contacts >= data.contacts.size())
			return 0;

		V3 pos_one = one.get_position();
		V3 pos_two = two.get_position();
		V3 midline = pos_one - pos_two;
		F size = glm::length(midline);

		if (size <= 0 || size >= one.radius + two.radius) 
			return 0;
		
		V3 normal = midline / size;

		Contact* c = new Contact();
		c->contact_normal = normal;
		c->contact_point = pos_one + .5f * midline;
		c->penetration = size - one.radius - two.radius;
		c->set_body_data(one.body, two.body, 1, 0);
		data.contacts.push_back(c);
		return 1;

	}


	U CollisionDetector::sphere_and_half_space(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData &data)
	{
		if (data.max_contacts >= data.contacts.size())
			return 0;

		V3 sphere_pos = sphere.get_position();
		//Formula to get distance of sphere from plane
		F sphere_dist = dot(plane.normal, sphere_pos) - sphere.radius - plane.offset;

		if (sphere_dist >= 0) 
			return 0;

		Contact* c = new Contact();
		c->contact_normal = plane.normal;
		c->contact_point = sphere_pos - plane.normal * (sphere_dist + sphere.radius);
		c->penetration = -sphere_dist;
		c->set_body_data(sphere.body, nullptr, 1, 0);
		data.contacts.push_back(c);
		return 1;
	}

	U CollisionDetector::sphere_and_true_plane(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData &data)
	{
		if (data.max_contacts >= data.contacts.size())
			return 0;

		V3 sphere_pos = sphere.get_position();
		//Formula to get distance of center of the sphere from plane
		F center_dist = dot(plane.normal, sphere_pos) - plane.offset;

		if (center_dist*center_dist >= sphere.radius * sphere.radius)
			return 0;

		V3 normal = plane.normal;
		F penetration = -center_dist;
		if (penetration < 0) {
			normal *= -1;
			penetration *= -1;
		}

		penetration += sphere.radius;

		Contact* c = new Contact();
		c->contact_normal = normal;
		c->contact_point = sphere_pos - plane.normal * center_dist;
		c->penetration = penetration;
		c->set_body_data(sphere.body, nullptr, 1, 0);
		data.contacts.push_back(c);
		return 1;
	}

	V3 Primitive::get_position() const
	{
		return glm::vec3(transform[3]);
	}

}