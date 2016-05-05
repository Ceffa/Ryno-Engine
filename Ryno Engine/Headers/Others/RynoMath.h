#pragma once
#include "Types.h"
#include "Structures.h"
#include <random>

//Constants definitions
#define M_HALF_PI 1.57079632679489661923
#define M_PI 3.14159265358979323846
#define M_PI_2 6.28318530717958647692
#define DEG_TO_RAD 0.0174532925199433

namespace Ryno{	
	class ryno_math{

	public:
		//Lerps
		static F32 lerp(F32 a, F32 b, F32 value);
		static glm::vec3 lerp(glm::vec3 a, glm::vec3 b, F32 value);
		static ColorRGBA lerp(ColorRGBA a, ColorRGBA b, F32 value);

		//Randoms
		static glm::vec3 get_rand_dir();
		static glm::vec3 get_rand_dir(F32 p_min, F32 p_max, F32 y_min, F32 y_max);
		static I32 rand_int_range(I32 min, I32 max);
		static F32 rand_float_range(F32 min, F32 max);
		static glm::vec3 rand_vec3_range(glm::vec3 min, glm::vec3 max);
		static ColorRGBA rand_color_range(ColorRGBA& min, ColorRGBA& max);


	private:
		//Random generators
		static std::random_device rd;
		static std::mt19937 mt;
	};
}
