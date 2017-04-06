#pragma once
#include "Types.h"
#include "RynoMath.h"
#include "Structures.h"


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
	struct WindowSize {
		static U32 w;
		static U32 h;
	};
	struct WindowPos {
		static U32 x;
		static U32 y;
	};
	struct WindowIndex {
		static U32 idx;
	};
}