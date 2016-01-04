#include "Mallocator.h"

namespace Ryno{

	Mallocator* Mallocator::instance = nullptr;


	Mallocator* Mallocator::get_instance(){

		if (instance == nullptr) {//only at the beginning
			instance = new Mallocator();
		}
		return instance;
	}

	void* Mallocator::alloc(size_t obj_size, U8 alignement)
	{
		void* ptr = malloc(obj_size + alignement);
		if (ptr == nullptr){
			std::cout << "Mallocator: Malloc Failed." << std::endl;
			return nullptr;
		}
#if MALLOCATOR_LOG
		std::cout << "Mallocator: allocating" << std::endl;
#endif

		return ptr;


	}

	void Mallocator::dealloc(void* p)
	{
		free(p);
#if MALLOCATOR_LOG
		std::cout << "Mallocator: deallocating" << std::endl;
#endif
	}



}