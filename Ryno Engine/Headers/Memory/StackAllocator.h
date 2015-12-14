#pragma once
#include "Types.h"
#include "Allocator.h"
#include <iostream>
#include <initializer_list>

#define STACK_ALLOCATOR_LOG 0
namespace Ryno{

	class StackAllocator : public Allocator{
	public:
		static StackAllocator* get_instance();

		bool init(U64 MB_size);
		
		//Deallocate top object. Use this instead of dealloc
		void pop();

		void wipe_all();//wipe memory
		void free_all();//free memory

	private:
		U64 size; //bytes
		void *start=nullptr, *current=nullptr, *end=nullptr;
		StackAllocator::StackAllocator(){}
		bool memory_full();
		//Allocate
		void* alloc(size_t obj_size, U8 alignement) override;
		//Does nothing: deallocation is handled by wipe. Do not use.
		void dealloc(void* p) override;
	};
}
