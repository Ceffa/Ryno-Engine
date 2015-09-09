#include <stdint.h>

//Size definitions
#define WINDOW_WIDTH 1366			
#define WINDOW_HEIGHT 768

//Types definitions
#define I8	int8_t
#define I16 int16_t
#define I32 int32_t
#define I64 int64_t

#define U8	uint8_t
#define U16 uint16_t
#define U32 uint32_t
#define U64 uint64_t

#define F32 float
#define F64 double
#define C char

//Constants definitions
#define M_HALF_PI 1.57079632679489661923
#define M_PI 3.14159265358979323846
#define M_PI_2 6.28318530717958647692
#define DEG_TO_RAD 0.0174532925199433

//Engine folder or game folder
#pragma once
enum LocationOfResource{
	ENGINE_FOLDER,
	GAME_FOLDER
};

static const C* BASE_PATHS[2]{"../Ryno Engine/", ""};
