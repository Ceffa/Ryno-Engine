#pragma once
#pragma once
#include "RigidBody.h"

namespace Ryno {

	struct PotentialContact {
	public:
		PotentialContact(RigidBody* a, RigidBody* b) {
			bodies[0] = a;
			bodies[1] = b;
		}
		RigidBody* bodies[2];
	};

	template<typename BoundingVolume>
	class BVH_node {
	public:

		BVH_node(BVH_node* _parent, RigidBody* _body, const BoundingVolume& _volume);

		//Two children because it's a binary tree
		BVH_node* children[2];

		//Bounding volume that encompasses all children
		BoundingVolume volume;

		//Might be null if not a leaf
		RigidBody* body = nullptr;

		//Parent in hierarchy. Might be null
		BVH_node* parent = nullptr;

		bool is_leaf() const;

		//Returns the number of contacts, and store
		std::vector<PotentialContact*> get_potential_contacts(U limit) const;

		//Returns the number of contacts, and store
		std::vector<PotentialContact*> get_potential_contacts_with(BVH_node* other, std::vector& contacts, U limit) const;

		//Check if overlaps another node
		bool overlaps(const BVH_node* other) const;

		void update_bounding_volume();

		void insert(RigidBody* new_body, const BoundingVolume& new_volume);


	};

	template<typename BoundingVolume>
	std::vector<PotentialContact*> BVH_node<BoundingVolume>::get_potential_contacts(U limit) const
	{
		std::vector<PotentialContact*> contacts;

		if (is_leaf() || limit == 0)
			return contacts;

		return children[0]->get_potential_contacts_with(children[1], contacts, limit);
	}

	template<typename BoundingVolume>
	bool BVH_node<BoundingVolume>::is_leaf() const
	{
		return (body != nullptr);
	}

	template<typename BoundingVolume>
	BVH_node<BoundingVolume>::BVH_node(BVH_node* _parent, RigidBody* _body, const BoundingVolume& _volume) : parent(_parent), body(_body), volume(_volume)
	{
		children[0] = children[1] = 0;
	}

	template<typename BoundingVolume>
	bool BVH_node<BoundingVolume>::overlaps(const BVH_node* other) const
	{
		return volume->overlaps(other->volume);
	}

	template<typename BoundingVolume>
	std::vector<PotentialContact*> BVH_node<BoundingVolume>::get_potential_contacts_with(BVH_node* other, std::vector& contacts, U limit) const
	{
		//Not overlapping -> return
		if (!overlaps(other) || limit == 0)
			return contacts;
		//Overlapping -> push and return
		if (is_leaf() && other->is_leaf()) {
			contacts.push_back(new PotentialContact(body, other->body));
			return contacts;
		}

		//Neither of those -> recursive.
		//Choose branch to descend into depending on size
		if (other->is_leaf() || (!is_leaf() && volume->get_size() >= other->volume->get_size())) {

			//Check if my children collide with my brother
			contacts = children[0]->get_potential_contacts_with(other, contacts, limit);

			if (limit > contacts.size())
				contacts = children[1]->get_potential_contacts_with(other, contacts, limit - contacts.size());
		}
		else {

			//Check if I collide with my brother's children
			contacts = get_potential_contacts_with(other->children[0], contacts, limit);

			if (limit > contacts.size())
				contacts = get_potential_contacts_with(other->children[1], contacts, limit - contacts.size());
		}

		return contacts;
	}

	template<typename BoundingVolume>
	void BVH_node<BoundingVolume>::update_bounding_volume()
	{
		if (is_leaf()) return;

		// Use the bounding volume combining constructor.
		volume = BoundingVolume(
			children[0]->volume,
			children[1]->volume
			);

		// Recurse up the tree
		if (parent) parent->update_bounding_volume();
	}

	template<typename BoundingVolume>
	void BVH_node<BoundingVolume>::insert(RigidBody* new_body, const BoundingVolume& new_volume)
	{
		//If root is leaf: spawn two children
		//child 0 is a copy of the root, child 2 is the new one
		if (is_leaf()) {
			children[0] = new BVH_node(this, body, volume);
			children[1] = new BVH_node(this, new_body, new_volume);
			//Delete data from the parent (because it's been copied)
			this->body = nullptr;
			update_bounding_volume();
		}
		else {
			//Assign new volume to the child that
			//will grow less with its addition
			if (children[0]->volume.get_growth(new_volume) < children[1]->volume.get_growth(new_volume))
				children[0]->insert(new_body, new_volume);
			else
				children[1]->insert(new_body, new_volume);
		}
	}
}