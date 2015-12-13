#pragma once
#include "Types.h"
#include "Allocator.h"
#include <iostream>
#include <initializer_list>
#include <unordered_map>

namespace Ryno{

	
	class Mallocator : public Allocator{
	public:
		static Mallocator* get_instance();

		void* alloc(size_t obj_size, U8 alignement) override;
		void dealloc(void* p) override;


	private:
		Mallocator::Mallocator(){}
	};
}