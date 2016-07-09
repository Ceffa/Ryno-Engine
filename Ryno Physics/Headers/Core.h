#pragma once
#include "Types.h"
#include "glm/glm.hpp"

#define PRECISION_32 

#ifdef PRECISION_32
#define V3 glm::vec3
#define F RynoEngine::F32
#elif PRECISION_64
#define V3 glm::dvec3
#define F RynoEngine::F64
#endif

