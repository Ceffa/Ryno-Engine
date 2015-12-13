#pragma once
#include "Types.h"
#include "Allocator.h"
#include <iostream>
#include <initializer_list>
#include <unordered_map>

namespace Ryno{

	
	class ReferenceAllocator : public Allocator{
	public:
		static ReferenceAllocator* get_instance();

		void* alloc(size_t obj_size, U8 alignement) override;
		void dealloc(void* p) override;


	private:
		ReferenceAllocator::ReferenceAllocator(){}
	};
}