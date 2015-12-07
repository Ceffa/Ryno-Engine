#include "StackAllocator.h"

namespace Ryno{

	StackAllocator* StackAllocator::get_instance(){

		static StackAllocator instance;//only at the beginning
		return &instance;
	}

	bool StackAllocator::init(U64 MB_size){
		size = MB_size * 1024;
		pers_start = (void*)malloc(size);
		if (pers_start == nullptr){
			std::cout << "Stack Allocator malloc failed:" << std::endl;
			return false;
		}
		pers_current = pers_start;
		temp_start = (void*)((U64)pers_start + size);
		temp_current = temp_start;
		
	}
}
