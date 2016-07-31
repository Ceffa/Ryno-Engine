#include "Narrow.h"
#include <GLM//gtc/matrix_access.hpp>
#include <iostream>

namespace Ryno {

	void CollisionData::setup(F _max_contacts)
	{
		max_contacts = _max_contacts;
		if (contacts)
			delete contacts;
		contacts = new Contact[max_contacts];
		clear();

	}

	

	void CollisionData::clear()
	{
		//reset pointer and contacts
		contacts -= (max_contacts - remaining_contacts);
		remaining_contacts = max_contacts;

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
		V3 sphere_center = V3(glm::inverse(box.transform) * V4(world_sphere_center,1));

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


	void Primitive::calculate_transform()
	{
		transform = offset * body->get_transform_matrix();
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

		return proj_a + proj_b - abs(dot(axis,center_to_center));
	}

	//Add a vertex - face contact for boxes
	void fill_point_face_box_box(
		const CollisionBox &one,
		const CollisionBox &two,
		const V3 &center_to_center,
		CollisionData *data,
		U best,
		F pen
		)
	{
		// This method is called when we know that a vertex from
		// box two is in contact with box one.

		Contact* c = data->contacts;

		// We know which axis the collision is on (i.e. best),
		// but we need to work out which of the two faces on
		// this axis.
		V3 normal = one.get_axis(best);
		if (dot(one.get_axis(best) , center_to_center) > 0)
		{
			normal *= -1;
		}

		// Work out which vertex of box two we're colliding with.
		// Using center_to_center doesn't work!
		V3 vertex = V3(
			(dot(two.get_axis(0), normal) < 0) ? two.half_size.x : -two.half_size.x,
			(dot(two.get_axis(1), normal) < 0) ? two.half_size.y : -two.half_size.y,
			(dot(two.get_axis(2), normal) < 0) ? two.half_size.z : -two.half_size.z
			);



		// Create the contact data
		c->contact_normal = normal;
		c->penetration = pen;
		c->contact_point = V3(glm::mat3x4(two.transform) * vertex);
		c->set_body_data(one.body, two.body,
			1,0);
		++data;
	}

	static inline bool try_axis(
		const CollisionBox &one,
		const CollisionBox &two,
		const V3& axis,
		const V3& center_to_center,
		U index,

		// These values may be updated
		F& smallest_penetration,
		unsigned &smallest_case
		)
	{
		// Make sure we have a normalized axis, and don't check almost parallel axes
		if (glm::length2(axis) < 0.0001) return true;
		glm::normalize(axis);

		F penetration = penetrationOnAxis(one, two, axis, center_to_center);
		if (penetration < 0) return false;
		if (penetration < smallest_penetration) {
			smallest_penetration = penetration;
			smallest_case = index;
		}
		return true;
	}

#define CHECK_OVERLAP(axis, index) \
    if (!try_axis(one, two, (axis), center_to_center, (index), pen, best)) return 0;



	//Gets the contact point given the axis and enter of the two edges.
	//It makes an average which takes into account the direction of the edges
	//relative to the direction of the vector dividing their centers.
	//I didn't get all of this code.
	static inline V3 get_contact_point(
		const V3 &pOne,
		const V3 &dOne,
		F oneSize,
		const V3 &pTwo,
		const V3 &dTwo,
		F twoSize,

		// If this is true, and the contact point is outside
		// the edge (in the case of an edge-face contact) then
		// we use one's midpoint, otherwise we use two's.
		bool useOne)
	{
		V3 toSt, cOne, cTwo;
		F dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
		F denom, mua, mub;

		

		//Vector between the two middle points of the edges
		toSt = pOne - pTwo;
		//Get portion of the vectors in direction of each edge
		dpStaOne = dot(dOne,toSt);
		dpStaTwo = dot(dTwo, toSt);
		//How far along each edge is closest point
		smOne = glm::length2(dOne);
		smTwo = glm::length2(dTwo);
		dpOneTwo = dot(dTwo, dOne);
		denom = smOne * smTwo - dpOneTwo * dpOneTwo;


		// Zero denominator indicates parrallel lines
		if (abs(denom) < 0.0001f) {
			return useOne ? pOne : pTwo;
		}

		mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
		mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

		// If either of the edges has the nearest point out
		// of bounds, then the edges aren't crossed, we have
		// an edge-face contact. Our point is on the edge, which
		// we know from the useOne parameter.
		if (mua > oneSize ||
			mua < -oneSize ||
			mub > twoSize ||
			mub < -twoSize)
		{
			return useOne ? pOne : pTwo;
		}
		else
		{
			cOne = pOne + dOne * mua;
			cTwo = pTwo + dTwo * mub;

			return cOne * 0.5f + cTwo * 0.5f;
		}
	}




	U CollisionDetector::box_and_box(const CollisionBox &one, const CollisionBox &two, CollisionData &data)
	{
		// Find the vector between the two centres
		V3 center_to_center = two.get_axis(3) - one.get_axis(3);

		// We start assuming there is no contact
		F pen = FLT_MAX;
		U best = UINT_MAX;

		// Now we check each axes, returning if it gives us
		// a separating axis, and keeping track of the axis with
		// the smallest penetration otherwise.

		//Note: for convex shapese the SAT says that we must try an axis for each face
		//and for each possible crossproduct of edges.
		//For cubes this is just 15 possible axis due to repetitions
		CHECK_OVERLAP(one.get_axis(0), 0);
		CHECK_OVERLAP(one.get_axis(1), 1);
		CHECK_OVERLAP(one.get_axis(2), 2);

		CHECK_OVERLAP(two.get_axis(0), 3);
		CHECK_OVERLAP(two.get_axis(1), 4);
		CHECK_OVERLAP(two.get_axis(2), 5);

		// Store the best axis-major, in case we run into almost
		// parallel edge collisions later
		unsigned bestSingleAxis = best;

		CHECK_OVERLAP(cross(one.get_axis(0), two.get_axis(0)), 6);
		CHECK_OVERLAP(cross(one.get_axis(0), two.get_axis(1)), 7);
		CHECK_OVERLAP(cross(one.get_axis(0), two.get_axis(2)), 8);
		CHECK_OVERLAP(cross(one.get_axis(1), two.get_axis(0)), 9);
		CHECK_OVERLAP(cross(one.get_axis(1), two.get_axis(1)), 10);
		CHECK_OVERLAP(cross(one.get_axis(1), two.get_axis(2)), 11);
		CHECK_OVERLAP(cross(one.get_axis(2), two.get_axis(0)), 12);
		CHECK_OVERLAP(cross(one.get_axis(2), two.get_axis(1)), 13);
		CHECK_OVERLAP(cross(one.get_axis(2), two.get_axis(2)), 14);
		
		if (best == UINT_MAX)
			return 0;

		if (best < 3)
		{
			// We've got a vertex of box two on a face of box one.
			fill_point_face_box_box(one, two, center_to_center, &data, best, pen);
			return 1;
		}
		else if (best < 6) {
			//Same thing if axis is parallel to a face of box 2
			fill_point_face_box_box(two, one, -center_to_center, &data, best - 3, pen);
			return 1;
		}
		else {
			//Edge-edge contact

			// We've got an edge-edge contact. Find out which axes
			//And calculate final axis with cross product
			best -= 6;
			U one_axis_idx = best / 3;
			U two_axis_idx = best % 3;
			V3 one_axis = one.get_axis(one_axis_idx);
			V3 two_axis = two.get_axis(two_axis_idx);
			V3 axis = glm::normalize(cross(one_axis, two_axis));

			// The axis should point from box one to box two.
			if (dot(axis , center_to_center) > 0) axis = axis * -1.0f;

			// We have the axes, but not the edges: each axis has 4 edges parallel
			// to it, we need to find which of the 4 for each object. We do
			// that by finding the point in the centre of the edge. We know
			// its component in the direction of the box's collision axis is zero
			// (its a mid-point) and we determine which of the extremes in each
			// of the other axes is closest.
			V3 pt_on_edge_one = one.half_size;
			V3 pt_on_edge_two = two.half_size;
			for (U i = 0; i < 3; i++)
			{
				if (i == one_axis_idx) pt_on_edge_one[i] = 0;
				else if (dot(one.get_axis(i) ,axis) > 0) pt_on_edge_one[i] = -pt_on_edge_one[i];

				if (i == two_axis_idx) pt_on_edge_two[i] = 0;
				else if (dot(two.get_axis(i), axis) < 0) pt_on_edge_two[i] = -pt_on_edge_two[i];
			}

			// Move them into world coordinates (they are already oriented
			// correctly, since they have been derived from the axes).
			pt_on_edge_one = V3(glm::mat3x4(one.transform) * pt_on_edge_one);
			pt_on_edge_two = V3(glm::mat3x4(two.transform) * pt_on_edge_two);

			// So we have a point and a direction for the colliding edges.
			// We need to find out point of closest approach of the two
			// line-segments.
			V3 vertex = get_contact_point(
				pt_on_edge_one, one_axis, one.half_size[one_axis_idx],
				pt_on_edge_two, two_axis, two.half_size[two_axis_idx],
				bestSingleAxis > 2
				);

			// We can fill the contact.
			Contact* c = data.contacts;

			c->penetration = pen;
			c->contact_normal = axis;
			c->contact_point = vertex;
			c->set_body_data(one.body, two.body,
				1,0);
			++data;
			return 1;

		}

	}
	
}