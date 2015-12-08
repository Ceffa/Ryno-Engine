#pragma once
#include "Types.h"
#include <iostream>
#include <initializer_list>

namespace Ryno{


	class StackAllocator{
	public:
		static StackAllocator* get_instance();

		bool init(U64 MB_size);


		template<typename... Args>
		void* temp_alloc(size_t obj_size, Args...args){
			U64 old_ptr = (U64)temp_current;
			U64 temp_ptr = old_ptr - obj_size;
			temp_current = (void*)(temp_ptr - (temp_ptr % sizeof(U64)) - sizeof(U64)) ; //alignement
			
			if (memory_full())
				return nullptr;

			*(U64*)temp_current = old_ptr;
			return (void*)((U64)temp_current + sizeof(U64));
		}

		template<typename... Args>
		void* pers_alloc(size_t obj_size, Args...args){
			U64 old_ptr = (U64)pers_current;
			U64 pers_ptr = old_ptr + obj_size;
			U64 pers_mod = pers_ptr % sizeof(U64);
			pers_current = (void*)(pers_ptr + sizeof(U64) + ((pers_mod == 0) ? 0 : (sizeof(U64) - pers_mod))); //alignement
			
			if (memory_full())
				return nullptr;

			*(U64*)((U64)pers_current - sizeof(U64)) = old_ptr;
			return (void*)old_ptr;
		}

		void free_temp_top();

	private:
		U64 size; //bytes
		void *pers_start, *pers_current, *temp_start, *temp_current;
		StackAllocator::StackAllocator(){}
		bool memory_full();
	};


}
