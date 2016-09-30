#pragma once
#include "Types.h"
#include "RynoMath.h"
#include "Structures.h"

//Size definitions
#define WINDOW_WIDTH 1000		
#define WINDOW_HEIGHT 450


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
	static const C* BASE_PATHS[2]{ "../Ryno Engine/", "../Game/" };
}