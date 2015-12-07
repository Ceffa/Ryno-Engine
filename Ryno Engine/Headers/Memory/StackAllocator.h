#pragma once
#include "Global.h"
#include <iostream>
#include <initializer_list>

namespace Ryno{

	class StackAllocator{
	public:
		static StackAllocator* get_instance();

		bool init(U64 MB_size);


		template<typename T, typename... Args>
		bool temp_alloc(T** t, Args...args){

			U64 obj_size = sizeof(T);
			U64 temp_ptr = (U64)temp_current - obj_size;
			temp_current = (void*)(temp_ptr - (temp_ptr % 4)); //alignement
			if (temp_current <= pers_current){
				std::cout << "Stack Allocator memory is full" << std::endl;
				std::cout << "Pers pointer at: " << pers_current << std::endl;
				std::cout << "Temp pointer at: " << temp_current << std::endl;
				return false;
			}
			*t = new (temp_current)T(args...);
			return true;
		}

		template<typename T, typename... Args>
		bool pers_alloc(T** t, Args...args){
			U64 obj_size = sizeof(T);
			U64 temp_ptr = (U64)temp_current + obj_size;
			U64 temp_mod = temp_ptr % 4;
			temp_ptr = temp_ptr + ((temp_mod == 0) ? 0 : (4 - temp_mod)); //alignement
			if ((void*)temp_ptr <= pers_current){
				std::cout << "Stack Allocator memory is full" << std::endl;
				std::cout << "Pers pointer at: " << pers_current << std::endl;
				std::cout << "Temp pointer at: " << temp_current << std::endl;
				return false;
			}
			*t = new (temp_current)T(args...);
			temp_current = (void*)temp_ptr;
			return true;
		}


	private:
		U64 size; //bytes
		void *pers_start, *pers_current, *temp_start, *temp_current;
		StackAllocator::StackAllocator(){}
	};


}
