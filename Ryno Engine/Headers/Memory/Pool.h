#pragma once
#include "Types.h"
#include <iostream>
#include "Allocator.h"
#include <list>

#define POOL_LOG 0
namespace Ryno{

	class Pool : public Allocator{
	public:
		Pool(){}
		~Pool();

		template <class T>
		bool init(U32 number)
		{
			U64 entry_size = sizeof(T) + __alignof(T);
			U64 size = number * entry_size;
			start = malloc(size);
			if (start == nullptr){
				std::cout << "Pool allocator: malloc failed" << std::endl;
				return false;
			}
			end = (void*)((U64)start + size);
			for (U32 i = 0; i < number; i++){
				free_blocks.push_back((void*)((U64)start + entry_size * i));
			}
#if POOL_LOG
			std::cout << "Pool allocator: initialized" << std::endl;
#endif
			return true;
		}

	private:
		void *start, *current, *end;
		void* alloc(size_t obj_size, U8 alignement) override;
		void dealloc(void* p) override;
		void free_all();
		void wipe_all();

		std::list<void*> free_blocks;
	};

	

}