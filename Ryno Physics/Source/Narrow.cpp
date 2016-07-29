#include "Narrow.h"
#include <GLM//gtc/matrix_access.hpp>
#include <iostream>

namespace Ryno {

	void CollisionData::setup()
	{
		if (contacts)
			delete contacts;
		contacts = new Contact[max_contacts];
	}

	

	U CollisionDetector::sphere_and_sphere(const CollisionSphere &one, const CollisionSphere &two, CollisionData& data)
	{
		if (data.remaining_contacts <=0)
			return 0;

		V3 pos_one = one.get_position();
		V3 pos_two = two.get_position();
		V3 midline = pos_one - pos_two;
		F size = glm::length(midline);

		if (size <= 0 || size >= one.radius + two.radius) 
			return 0;
		
		V3 normal = midline / size;

		Contact* c = data.contacts;
		c->contact_normal = normal;
		c->contact_point = pos_one + .5f * midline;
		c->penetration = size - one.radius - two.radius;
		c->set_body_data(one.body, two.body, 1, 0);
		++data;
		return 1;

	}


	U CollisionDetector::sphere_and_half_space(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData &data)
	{
		if (data.remaining_contacts<=0)
			return 0;

		V3 sphere_pos = sphere.get_position();
		//Formula to get distance of sphere from plane
		F sphere_dist = dot(plane.normal, sphere_pos) - sphere.radius - plane.offset;

		if (sphere_dist >= 0) 
			return 0;

		Contact* c = data.contacts;
		c->contact_normal = plane.normal;
		c->contact_point = sphere_pos - plane.normal * (sphere_dist + sphere.radius);
		c->penetration = -sphere_dist;
		c->set_body_data(sphere.body, nullptr, 1, 0);
		++data;
		return 1;
	}

	U CollisionDetector::sphere_and_true_plane(const CollisionSphere &sphere, const CollisionPlane &plane, CollisionData &data)
	{
		if (data.remaining_contacts<=0)
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

		Contact* c = data.contacts;
		c->contact_normal = normal;
		c->contact_point = sphere_pos - plane.normal * center_dist;
		c->penetration = penetration;
		c->set_body_data(sphere.body, nullptr, 1, 0);
		++data;
		return 1;
	}

	U CollisionDetector::box_and_half_space(const CollisionBox &box, const CollisionPlane &plane, CollisionData &data)
	{
		if (data.remaining_contacts <=0)
			return 0;
		
		static F mults[8][3] = { { 1,1,1 },{ -1,1,1 },{ 1,-1,1 },{ -1,-1,1 },
		{ 1,1,-1 },{ -1,1,-1 },{ 1,-1,-1 },{ -1,-1,-1 } };

		U contact_used = 0;

		for (I i = 0; i < 8; i++) {

			V3 vert_pos = box.half_size * V3(glm::mat3x4(box.transform)  * V3(mults[i][1], mults[i][2], mults[i][3]));
			F vert_dist = dot(vert_pos , plane.normal) - plane.offset;

			if (vert_dist <= 0) {

				Contact* c = data.contacts;

				//contact point halfway between plane and vertex
				c->penetration = plane.offset - vert_dist;
				c->contact_point = plane.normal * -c->penetration + vert_pos;
				c->contact_normal = plane.normal;
				c->set_body_data(box.body, nullptr, 1, 0);
				++data;
				contact_used++;
				if (data.remaining_contacts <= 0)
					return contact_used;

			}
		}
		return contact_used;
	
	}


	U CollisionDetector::box_and_sphere(const CollisionBox &box, const CollisionSphere &sphere, CollisionData &data)
	{
		V3 world_sphere_center = sphere.get_position();

		//Get sphere in box local coords
		V3 sphere_center = V3(glm::inverse(box.transform) * V4(sphere_center,1));

		// Early out check to see if we can exclude the contact
		if (abs(sphere_center.x) - sphere.radius > box.half_size.x ||
			abs(sphere_center.y) - sphere.radius > box.half_size.y ||
			abs(sphere_center.z) - sphere.radius > box.half_size.z)
		{
			return 0;
		}

		//Get closest point to center of sphere.
		//It is done by clamping the center inside the box
		V3 closest_point = V3(
			glm::clamp(sphere_center.x, -box.half_size.x, box.half_size.x),
			glm::clamp(sphere_center.y, -box.half_size.y, box.half_size.y),
			glm::clamp(sphere_center.z, -box.half_size.z, box.half_size.z)
			);

		F dist = glm::length2(closest_point - sphere_center);
		if (dist > sphere.radius * sphere.radius)
			return 0;

		//Get closest point in world coords 
		//because thisis what the contact requires
		closest_point = V3(box.transform * V4(closest_point, 1));

		Contact* c = data.contacts;
		c->set_body_data(box.body, sphere.body, 1, 0);
		c->contact_normal = glm::normalize(closest_point - world_sphere_center);
		c->contact_point = closest_point;
		c->penetration = sphere.radius - sqrt(dist);
		++data;
		return 1;
	}

	bool IntersectionTest::box_and_half_space(const CollisionBox &box, const CollisionPlane &plane)
	{
		return true;
	}


	V3 Primitive::get_axis(U axis) const
	{
		return glm::vec3(transform[axis]);
	}


	//Returns the half length the box when projected on the axis.
	//It is done by adding the projected components of the 3 direction
	static inline F transformToAxis(const CollisionBox& box, const V3& axis) {
		
		return
			abs(dot(axis, box.get_axis(0))) * box.half_size.x +
			abs(dot(axis, box.get_axis(1))) * box.half_size.y +
			abs(dot(axis, box.get_axis(2))) * box.half_size.z;
			
	}
	//This function returns the amount of interpenetration
	//between the two boxes on a certain axes.
	//If negative, they are not penetrating
	static inline F penetrationOnAxis(
		const CollisionBox& a,
		const CollisionBox& b,
		const V3 axis,
		const V3 center_to_center
		) 
	{
		F proj_a = transformToAxis(a, axis);
		F proj_b = transformToAxis(b, axis);

		return proj_a + proj_b - dot(axis,center_to_center);
	}


}