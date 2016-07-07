#pragma once
#include "Types.h"
#include "RynoMath.h"
#include "Structures.h"

//Size definitions
#define WINDOW_WIDTH 1920				
#define WINDOW_HEIGHT 1080

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


static const C* BASE_PATHS[2]{"../Ryno Engine/", ""};
