#include "Mallocator.h"

namespace Ryno{

	Mallocator* Mallocator::get_instance(){

		static Mallocator instance;//only at the beginning
		return &instance;
	}

	void* Mallocator::alloc(size_t obj_size, U8 alignement)
	{
		void* ptr = malloc(obj_size + alignement);
		if (ptr == nullptr){
			std::cout << "Reference Allocator: Malloc Failed." << std::endl;
			return nullptr;
		}
		return ptr;


	}

	void Mallocator::dealloc(void* p)
	{
		free(p);
	}

}
