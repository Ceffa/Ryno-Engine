//#include "StackAllocator.h"
//
//namespace Ryno{
//
//	StackAllocator* StackAllocator::get_instance(){
//
//		static StackAllocator instance;//only at the beginning
//		return &instance;
//	}
//
//	bool StackAllocator::init(U64 MB_size){
//		size = MB_size * 1024;
//		pers_start = (void*)malloc(size);
//		if (pers_start == nullptr){
//			std::cout << "Stack Allocator malloc failed:" << std::endl;
//			return false;
//		}
//		pers_current = pers_start;
//		temp_start = (void*)((U64)pers_start + size);
//		temp_current = temp_start;
//		
//	}
//
//	void StackAllocator::free_temp_top()
//	{
//		void* previous = *(void**)temp_current;
//		std::cout << previous << "  " << temp_current <<  std::endl;
//		//delete  ((U64*)temp_current + sizeof(void*)); //FUUUUCK
//		temp_current = previous;
//	}
//
//	bool StackAllocator::memory_full()
//	{
//		if (temp_current <= pers_current){
//			std::cout << "Stack Allocator memory is full" << std::endl;
//			std::cout << "Pers pointer at: " << pers_current << std::endl;
//			std::cout << "Temp pointer at: " << temp_current << std::endl;
//			return true;
//		}
//		return false;
//	}
//
//}
