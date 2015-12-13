#pragma once

#include <iostream>

#define LOG 1

namespace Ryno{

	class Allocator{
	public:
		virtual void* alloc(size_t size, U8 alignement) = 0;
		virtual void dealloc(void*) = 0;
	};

}