#include "Pool.h"


namespace Ryno{
	Pool::~Pool()
	{
#if POOL_LOG
		std::cout << "Pool allocator: destroying" << std::endl;
#endif
		free_all();
	}

	

	void* Pool::alloc(size_t obj_size, U8 alignement)
	{
		void* return_value = free_blocks.back();
		if (!return_value){
			std::cout << "Pool allocator: pool empty" << std::endl;
			return nullptr;
		}
		free_blocks.pop_back();
#if POOL_LOG
		std::cout << "Pool allocator: alloc" << std::endl;
#endif
		return return_value;
	}

	void Pool::dealloc(void* p)
	{
		if (p >= start && p < end){
			free_blocks.push_back(p);
		}
		else{
			std::cout << "Pool allocator: pool empty" << std::endl;
		}
	}

	void Pool::free_all()
	{
		free(start);
#if POOL_LOG
		std::cout << "Pool allocator: freeing memory" << std::endl;
#endif
	}

	void Pool::wipe_all()
	{
		free_blocks.clear();
#if POOL_LOG
		std::cout << "Pool allocator: wiping memory" << std::endl;
#endif
	}

}