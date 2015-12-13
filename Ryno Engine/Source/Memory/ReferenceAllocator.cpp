#include "ReferenceAllocator.h"

namespace Ryno{

	ReferenceAllocator* ReferenceAllocator::get_instance(){

		static ReferenceAllocator instance;//only at the beginning
		return &instance;
	}

	void* ReferenceAllocator::alloc(size_t obj_size, U8 alignement)
	{
		void* ptr = malloc(obj_size + alignement);
		if (ptr == nullptr){
			std::cout << "Reference Allocator: Malloc Failed." << std::endl;
			return nullptr;
		}
		return ptr;


	}

	void ReferenceAllocator::dealloc(void* p)
	{
		free(p);
	}

}
