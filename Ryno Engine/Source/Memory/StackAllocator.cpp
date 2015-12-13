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
//		start = (void*)malloc(size);
//		if (start == nullptr){
//			std::cout << "Stack Allocator malloc failed:" << std::endl;
//			return false;
//		}
//		current = start;
//		end = (void*)((U64)start + size);
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
//		if (current >= end){
//			std::cout << "Stack Allocator memory is full" << std::endl;
//			return true;
//		}
//		return false;
//	}
//
//}
