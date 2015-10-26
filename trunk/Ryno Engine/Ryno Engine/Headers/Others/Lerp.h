#pragma once
#include "Global.h"
#include "Structures.h"
namespace Ryno{

	F32 lerp(F32 a, F32 b, F32 value);

	glm::vec3 lerp(glm::vec3 a, glm::vec3 b, F32 value);
	
	ColorRGBA lerp(ColorRGBA a, ColorRGBA b, F32 value);

}