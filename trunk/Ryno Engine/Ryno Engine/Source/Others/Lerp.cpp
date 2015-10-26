#include "Lerp.h"
namespace Ryno{

	F32 lerp(F32 a, F32 b, F32 value){
		return (1.0f - value) * a + value * b;
	}

	glm::vec3 lerp(glm::vec3 a, glm::vec3 b, F32 value){
		return glm::vec3(lerp(a.x, b.x, value), lerp(a.y, b.y, value), lerp(a.z, b.z, value));
	}

	ColorRGBA lerp(ColorRGBA a, ColorRGBA b, F32 value){
		ColorRGBA ret;
		ret.lerp(a, b, value);
		return ret;
	}

}