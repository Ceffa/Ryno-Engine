#include "Allocator.h"

namespace Ryno{

	StackAllocator* Allocator::stack_allocator;
	ReferenceAllocator* Allocator::reference_allocator;
	PoolAllocator* Allocator::pool_allocator;
}
