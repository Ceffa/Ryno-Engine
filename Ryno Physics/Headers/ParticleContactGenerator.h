#pragma once
#include "Core.h"


namespace RynoEngine {

	class ParticleContact;
	class Particle;

	//Base class for contact generators
	class ParticleContactGenerator {
	public:
		//Fills the contact pointer with 0 to num_contacts contacts.
		//The number of contacts is put inside num_contacts
		virtual ParticleContact* add_contact(U max_contacts, U* num_contacts) = 0;
	
	};

	//Holds two particles, and can get the distance between them.
	//It doesn't know yet how to add a contact, it must be extended
	class ParticleLink : ParticleContactGenerator {
	public:
		Particle* particles[2];
	protected:
		F get_current_length() const;
	};

	//It creates contact when a link is longer then a predefined distance.
	//This makes it behave like a cable
	class ParticleCable : ParticleLink {
	public:
		ParticleCable(Particle* a, Particle* b, F length, F restit);
		F max_length;
		F restitution;
		virtual ParticleContact* add_contact(U max_contacts, U* num_contacts) override;


	};

	//It creates a stiff rod that keeps particle separated.
	//It does that by zeroing restitution, and resolves interpenetrations only
	class ParticleRod : ParticleLink {
	public:
		ParticleRod(Particle* a, Particle* b, F length);
		F max_length;
		virtual ParticleContact* add_contact(U max_contacts, U* num_contacts) override;


	};



}