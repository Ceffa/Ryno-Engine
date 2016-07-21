#pragma once
#include "Types.h"
#include "RynoMath.h"
#include "Structures.h"

//Size definitions
#define WINDOW_WIDTH 800				
#define WINDOW_HEIGHT 600

//Engine folder or game folder
#pragma once
enum Owner{
	ENGINE,
	GAME
};

enum Use{
	GUI,
	SHELL
};

namespace Ryno {
	static const C* BASE_PATHS[2]{ "../Ryno Engine/", "" };
}