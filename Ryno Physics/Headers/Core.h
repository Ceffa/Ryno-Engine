#pragma once
#include "Types.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

#define PRECISION_32 

#ifdef PRECISION_32
#define V3 glm::vec3
#define F RynoEngine::F32
#define U RynoEngine::U32
#define I RynoEngine::I32
#elif PRECISION_64
#define U RynoEngine::U64
#define I RynoEngine::I64
#define V3 glm::dvec3
#define F RynoEngine::F64
#endif

