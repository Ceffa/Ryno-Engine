#pragma once
#include "Types.h"
#include "RynoMath.h"
#include "Structures.h"
#include "Allocator.h"

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


static const C* BASE_PATHS[2]{"../Ryno Engine/", ""};


template<typename T>
class New {
	T* data = nullptr;

public:

	New(T* t) : data(t){}
	New() : data(nullptr){}
	T* operator->() const{ return data; }
	T* operator* () const { return data; }
	bool operator== (const New<T>& b) {
		return data == b.data;
	}

	~New() { delete data; }
};