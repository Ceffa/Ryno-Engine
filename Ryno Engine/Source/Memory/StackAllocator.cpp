#include "StackAllocator.h"

namespace Ryno{

	StackAllocator* StackAllocator::get_instance(){

		static StackAllocator instance;//only at the beginning
		return &instance;
	}

	bool StackAllocator::init(U64 MB_size){
		//If a stack is already allocated is freed.
		//So don't init twice unless you are sure
		if (memory_full())
			free_all();
		size = MB_size * 1024;
		start = (void*)malloc(size);
		if (start == nullptr){
			std::cout << "Stack Allocator: Malloc failed:" << std::endl;
			return false;
		}
		current = start;
		end = (void*)((U64)start + size);
#if STACK_ALLOCATOR_LOG
		std::cout << "Stack Allocator: created chunk of " << MB_size << " MB" << std::endl;
#endif
		
	}

	void* StackAllocator::alloc(size_t obj_size, U8 alignement)
	{
		void* allocate_at = current;
		void* address = (void*)((U64)current + obj_size + alignement);
		current = (void*)(sizeof(void*) + (U64)address);
		if (memory_full())
			return nullptr; 
		*(void**)address = allocate_at;
#if STACK_ALLOCATOR_LOG
		std::cout << "Stack Allocator: allocated at: " << current << std::endl;
#endif
		return allocate_at;
	}

	void StackAllocator::dealloc(void* p)
	{
#if STACK_ALLOCATOR_LOG
		std::cout << "Stack Allocator: deallocating... which does nothing " << std::endl;
#endif
	}

	void StackAllocator::pop(){
		void* last_addr_ptr = (void*)((U64)current - sizeof(void*));
		if (last_addr_ptr < start){
			std::cout << "Stack Allocator: nothing to pop" << std::endl;
			return;
		}
		
		void* last_addr = *(void**)last_addr_ptr;
#if STACK_ALLOCATOR_LOG
		std::cout << "Stack Allocator: pop at: " << current << std::endl;
#endif
		current = last_addr;
	}

	void StackAllocator::wipe_all()
	{
		current = start;
#if STACK_ALLOCATOR_LOG 
		std::cout << "Stack allocator: Wiping memory" << std::endl;
#endif
	}

	void StackAllocator::free_all(){
		free(start);
#if STACK_ALLOCATOR_LOG
		std::cout << "Stack allocator: Freeing memory chunk" << std::endl;
#endif
	}


	bool StackAllocator::memory_full()
	{
		if ((U64)current > (U64)end){
			std::cout << "Stack Allocator memory is full" << std::endl;
			return true;
		}
		return false;
	}

}
