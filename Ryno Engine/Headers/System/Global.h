#pragma once
#include "Types.h"
#include "RynoMath.h"
#include "Structures.h"

//Size definitions
#define WINDOW_WIDTH 800				
#define WINDOW_HEIGHT 600

//Engine folder or game folder
#pragma once
enum LocationOfResource{
	ENGINE_FOLDER,
	GAME_FOLDER
};

enum Use{
	GUI,
	SHELL
};


static const C* BASE_PATHS[2]{"../Ryno Engine/", ""};
