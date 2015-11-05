#include "Forces.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
glm::vec3 WeightForce::get_force()
{
	return  m * a;
}

glm::vec3 DragForce::get_force()
{
	
	return 0.5f * p * v * v * -dir * Cd * A;
}
