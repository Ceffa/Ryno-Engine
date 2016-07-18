#pragma once
#include "Core.h"



namespace RynoEngine {

	class RigidBody;
	//Abstract class. Each derived class is a
	//generator of a different type of force.
	//A particle must be registered with a Generator
	//through the ParticleForceRegister class
	class ForceGenerator {

	public: 
		
		virtual void update_force(RigidBody* p, F duration) = 0;

	};


	//Gravity
	//F = m * g
	class Gravity : public ForceGenerator {

	protected:
		V3 acceleration;
	public:
		Gravity(const V3& gravity) : acceleration(gravity) {}
		void update_force(RigidBody* particle, F duration) override;
	};

	//Drag
	//F: -norm(v) * (k1 * magn(v) + k2 * magn(v) * magn(v)   
	class Drag : public ForceGenerator {

	protected:
		F k1, k2;	//Drag coefficients
	public:
		Drag(F _k1, F _k2) : k1(_k1), k2(_k2){}
		void update_force(RigidBody* particle, F duration) override;
	};


	//Spring (two sides)
	//F: -k * delta(l)
	class Spring : public ForceGenerator {

	protected:
		//point of attach of the spring. They are local
		V3 attach_point_1, attach_point_2;
		RigidBody* other;	//Particle at the other extremity
		F k;				//Spring constant
		F rest_length;		//Rest length
		bool bungee;
	public:
		Spring(const V3& attach_1, const V3& attach_2, RigidBody* _other, F _k, F _rest_length, bool _bungee) : attach_point_1(attach_1),attach_point_2(attach_2), other(_other), k(_k), rest_length(_rest_length), bungee(_bungee) {}

		void update_force(RigidBody* particle, F duration) override;
	};


	//Spring (two sides)
	//F: -k * delta(l)
	class AnchoredSpring : public ForceGenerator {

	protected:
		V3 origin;			//Anchored position
		V3 attach_point;	//local point of attach in the object
		F k;				//Spring constant
		F rest_length;		//Rest length
		bool bungee;
	public:
		AnchoredSpring(const V3& _attach, const V3& _origin, F _k, F _rest_length, bool _bungee) :attach_point(_attach), origin(_origin), k(_k), rest_length(_rest_length), bungee(_bungee) {}

		void update_force(RigidBody* particle, F duration) override;
	};

	//Buoyancy (simplified, it's almost a spring)
	//F: 0 if outside, k*d id submerged,
	class Buoyancy : public ForceGenerator {

	protected:
		F level;			//Liquid (water) level
		F max_depth;		//Size of the object
		F liquid_density;	//Density of the liquid
		F volume;			//Volume of the object
	public:
		Buoyancy(F _level, F _depth, F _volume, F _liquid_density = 1000.0) : level(_level), max_depth(_depth), volume(_volume), liquid_density(_liquid_density) {}

		void update_force(RigidBody* particle, F duration) override;
	};
}