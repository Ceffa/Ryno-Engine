#pragma once
#include "Types.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Log.h"
#include <iostream>

#define PRECISION_32 

#define Q glm::quat
#define M3 glm::mat3
#define M4 glm::mat4

#ifdef PRECISION_32
#define V3 glm::vec3
#define V4 glm::vec4
#define F Ryno::F32
#define U Ryno::U32
#define I Ryno::I32
#elif PRECISION_64
#define U Ryno::U64
#define I Ryno::I64
#define V3 glm::dvec3
#define V4 glm::dvec4
#define F Ryno::F64
#endif

