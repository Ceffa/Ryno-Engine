#pragma once

#include <iostream>

#define LOG 1

namespace Ryno{

	class Allocator{
		template<class T> friend class New;
	protected:
		virtual void* alloc(size_t size, U8 alignement) = 0;
		virtual void dealloc(void*) = 0;
	};

}