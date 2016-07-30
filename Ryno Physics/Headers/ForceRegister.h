#pragma once
#include "Core.h"
#include "ForceGenerator.h"
#include <vector>;

namespace Ryno {

	class ForceRegister {

	protected:
		struct RegisterEntry {
			RegisterEntry(RigidBody* _body, ForceGenerator*fg) : body(_body), generator(fg) {}
			RigidBody* body;
			ForceGenerator* generator;
		};
		
		typedef std::vector<RegisterEntry> Registry;
		Registry registrations;

	public: 
		void add(RigidBody* body, ForceGenerator* fg);
		void remove(RigidBody* body, ForceGenerator* fg);
		void clear();
		void update_forces(F duration);

	};

}