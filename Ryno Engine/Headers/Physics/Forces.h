#include <GLM/glm.hpp>
#include "Global.h"


class IForce {
public:
	IForce(){}

	virtual glm::vec3 get_force() = 0;
};

class WeightForce : public IForce {
public:
	WeightForce(){}

	F32 m;		//Mass
	glm::vec3 a;		//Acceleration

	glm::vec3 get_force() override;
};

class DragForce : public IForce{
public:
	DragForce(){}

	F32 p;	//Density
	F32 v;		//Velocity
	glm::vec3 dir;//Direction
	F32 A;		//Frontal area
	F32 Cd;		//Drag coefficient

	glm::vec3 get_force() override;


};