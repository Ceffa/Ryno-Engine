#include "ParticleContactGenerator.h"
#include "Particle.h"
#include "ParticleContact.h"

namespace RynoEngine {

	
	F ParticleLink::get_current_length() const
	{
		return glm::length(particles[1]->get_position() - particles[0]->get_position());
	}

	ParticleCable::ParticleCable(Particle* a, Particle* b, F length, F restit) : max_length(length), restitution(restit)
	{
		particles[0] = a;
		particles[1] = b;
	}

	ParticleContact* ParticleCable::add_contact(U max_contacts, U* num_contacts)
	{
		F length = get_current_length();

		if (length < max_length) {
			*num_contacts = 0;
			return nullptr;
		}

		ParticleContact* contact = new ParticleContact();
		
		contact->penetration = length - max_length;
		contact->restitution = restitution;
		contact->particles[0] = particles[0];
		contact->particles[1] = particles[1];
		contact->contact_normal = normalize(particles[1]->get_position() - particles[0]->get_position());

		*num_contacts = 1;
		return contact;
	}

	ParticleRod::ParticleRod(Particle* a, Particle* b, F length) : max_length(length)
	{
		particles[0] = a;
		particles[1] = b;
	}

	ParticleContact* ParticleRod::add_contact(U max_contacts, U* num_contacts)
	{
		F length = get_current_length();

		if (length == max_length) {
			*num_contacts = 0;
			return nullptr;
		}

		ParticleContact* contact = new ParticleContact();

		contact->restitution = 0;
		contact->particles[0] = particles[0];
		contact->particles[1] = particles[1];

		//if longer than link...
		contact->penetration = length - max_length;
		contact->contact_normal = normalize(particles[1]->get_position() - particles[0]->get_position());
		//...if shorter
		if (length < max_length) {
			contact->penetration *= -1;
			contact->contact_normal *= -1;
		}


		*num_contacts = 1;
		return contact;
	}

}