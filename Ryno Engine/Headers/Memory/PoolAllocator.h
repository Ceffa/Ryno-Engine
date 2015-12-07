#pragma once
#include "Types.h"
#include <iostream>
#include <initializer_list>

namespace Ryno{

	class PoolAllocator{
	public:
		static PoolAllocator* get_instance();

		bool init();

	private:

		PoolAllocator::PoolAllocator(){}
	};
}