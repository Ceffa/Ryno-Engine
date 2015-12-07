#include "PoolAllocator.h"

namespace Ryno{


	PoolAllocator* PoolAllocator::get_instance(){

		static PoolAllocator instance;//only at the beginning
		return &instance;
	}

	bool PoolAllocator::init(){
		return true;
	}
}
