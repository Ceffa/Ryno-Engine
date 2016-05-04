#pragma once
#include "Types.h"
#include "RynoMath.h"
#include "Structures.h"
#include "Allocator.h"

//Size definitions
#define WINDOW_WIDTH 1000		
#define WINDOW_HEIGHT 700

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


namespace Ryno{
	template<typename T>
	class New {
	private:
		

	public:
		T* data = nullptr;
		Allocator* allocator;

		template<typename ...Args>
		T* create(Allocator* all, Args... args){
			allocator = all;
			data = new (all->alloc(sizeof(T),__alignof(T))) T(args...);
			if (data)
				return data;
			std::cout << "Problem allocating data." << std::endl;
			return nullptr;
		}
		template<typename ...Args>
		T* copy(const New<T>& old, Args... args){
			allocator = old.allocator;
			data = new (allocator->alloc(sizeof(T), __alignof(T))) T(**old,args...);
			if (data)
				return data;
			return nullptr;
		}
		T* operator->() const{ return data; }
		T* operator* () const { return data; }
		bool operator== (const New<T>& b) {
			return data == b.data;
		}
		

		~New<T>() { 
			if (data!=nullptr){
				data->~T();
				allocator->dealloc((void*)data);
				data = nullptr;
			}
		}
	};
}