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
			U64 temp_ptr = (U64)temp_current - obj_size;
			temp_current = (void*)(temp_ptr - (temp_ptr % 4)); //alignement
			if (temp_current <= pers_current){
				std::cout << "Stack Allocator memory is full" << std::endl;
				std::cout << "Pers pointer at: " << pers_current << std::endl;
				std::cout << "Temp pointer at: " << temp_current << std::endl;
				return false;
			}
			return temp_current;
		}

		template<typename... Args>
		void* pers_alloc(size_t obj_size, Args...args){
			U64 pers_ptr = (U64)pers_current + obj_size;
			U64 pers_mod = pers_ptr % 4;
			pers_ptr = pers_ptr + ((pers_mod == 0) ? 0 : (4 - pers_mod)); //alignement
			if ((void*)pers_ptr <= pers_current){
				std::cout << "Stack Allocator memory is full" << std::endl;
				std::cout << "Pers pointer at: " << pers_current << std::endl;
				std::cout << "Temp pointer at: " << temp_current << std::endl;
				return false;
			}
			void* instantiate_at = pers_current;
			pers_current = (void*)pers_ptr;
			return instantiate_at;
		}


	private:
		U64 size; //bytes
		void *pers_start, *pers_current, *temp_start, *temp_current;
		StackAllocator::StackAllocator(){}
	};


}
