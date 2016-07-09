#include "Particle.h"
#define NDEBUG
namespace RynoPhysics {

	void Particle::integrate(F duration)
	{
		if (inverse_mass <= 0.0) return;		//Check invalid mass

		assert(duration > 0.0);					//Assert valid duration

		position += velocity * duration;		//Increment position
		velocity += acceleration * duration;	//Increment velocity
		velocity *= pow(damping, duration);		//Damp velocity by factor d^t
	}

}