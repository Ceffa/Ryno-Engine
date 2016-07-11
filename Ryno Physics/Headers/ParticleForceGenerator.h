#pragma once
#include "Core.h"



namespace RynoEngine {

	class Particle;
	//Abstract class. Each derived class is a
	//generator of a different type of force.
	//A particle must be registered with a Generator
	//through the ParticleForceRegister class
	class ParticleForceGenerator {

	public: 
		
		virtual void update_force(Particle* p, F duration) = 0;

	};


	//Gravity
	//F = m * g
	class ParticleGravity : public ParticleForceGenerator {

	protected:
		V3 acceleration;
	public:
		ParticleGravity(const V3& gravity) : acceleration(gravity) {}
		void update_force(Particle* particle, F duration) override;
	};

	//Drag
	//F: -norm(v) * (k1 * magn(v) + k2 * magn(v) * magn(v)   
	class ParticleDrag : public ParticleForceGenerator {

	protected:
		F k1, k2;	//Drag coefficients
	public:
		ParticleDrag(F _k1, F _k2) : k1(_k1), k2(_k2){}
		void update_force(Particle* particle, F duration) override;
	};


	//Spring (two sides)
	//F: -k * delta(l)
	class ParticleSpring : public ParticleForceGenerator {

	protected:
		Particle* other;	//Particle at the other extremity
		F k;				//Spring constant
		F rest_length;		//Rest length
		bool bungee;
	public:
		ParticleSpring(Particle* _other, F _k, F _rest_length, bool _bungee) : other(_other), k(_k), rest_length(_rest_length), bungee(_bungee) {}

		void update_force(Particle* particle, F duration) override;
	};


	//Spring (two sides)
	//F: -k * delta(l)
	class ParticleAnchoredSpring : public ParticleForceGenerator {

	protected:
		V3 origin;			//Anchored position
		F k;				//Spring constant
		F rest_length;		//Rest length
		bool bungee;
	public:
		ParticleAnchoredSpring(const V3& _origin, F _k, F _rest_length, bool _bungee) : origin(_origin), k(_k), rest_length(_rest_length), bungee(_bungee) {}

		void update_force(Particle* particle, F duration) override;
	};

	//Buoyancy (simplified, it's almost a spring)
	//F: 0 if outside, k*d id submerged,
	class ParticleBuoyancy : public ParticleForceGenerator {

	protected:
		F level;			//Liquid (water) level
		F max_depth;		//Size of the object
		F liquid_density;	//Density of the liquid
		F volume;			//Volume of the object
	public:
		ParticleBuoyancy(F _level, F _depth, F _volume, F _liquid_density = 1000.0) : level(_level), max_depth(_depth), volume(_volume), liquid_density(_liquid_density) {}

		void update_force(Particle* particle, F duration) override;
	};
}