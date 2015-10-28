#include "RynoMath.h"


namespace Ryno{

	std::random_device ryno_math::rd;
	std::mt19937 ryno_math::mt(rd());
	
	
	F32 ryno_math::lerp(F32 a, F32 b, F32 value){
		return (1.0f - value) * a + value * b;
	}

	glm::vec3 ryno_math::lerp(glm::vec3 a, glm::vec3 b, F32 value){
		return glm::vec3(lerp(a.x, b.x, value), lerp(a.y, b.y, value), lerp(a.z, b.z, value));
	}

	ColorRGBA ryno_math::lerp(ColorRGBA a, ColorRGBA b, F32 value){
		ColorRGBA ret;
		ret.r = lerp(a.r, b.r, value);
		ret.g = lerp(a.g, b.g, value);
		ret.b = lerp(a.b, b.b, value);
		ret.a = lerp(a.a, b.a, value);

		return ret;
	}

	//UTILITIES
	glm::vec3 ryno_math::get_rand_dir(){
		return get_rand_dir(0, 360, 0, 180);
	}

	glm::vec3 ryno_math::get_rand_dir(F32 p_min, F32 p_max, F32 y_min, F32 y_max){
		F32 pitch = rand_float_range(p_min, p_max) * DEG_TO_RAD;
		F32 yaw = rand_float_range(y_min, y_max) * DEG_TO_RAD;
		F32 x = sin(yaw);
		F32 y = -cos(yaw)*sin(pitch);
		F32 z = -cos(pitch)*cos(yaw);
		return normalize(glm::vec3(x, y, z));
	}

	U32 ryno_math::rand_int_range(U32 min, U32 max){
		return (U32)rand_float_range(min, max);
	}

	F32 ryno_math::rand_float_range(F32 min, F32 max){
		std::uniform_real_distribution<float> f_dist(min, max);
		return f_dist(mt);
	}
	


}