//#pragma once
//#include "Types.h"
//#include <iostream>
//#include <initializer_list>
//
//namespace Ryno{
//
//	class StackAllocator{
//	public:
//		static StackAllocator* get_instance();
//
//		bool init(U64 MB_size);
//
//		template<typename T, typename... Args>
//		T* alloc(T*t, Args...args){
//			void* old_ptr = current;
//			U64 pers_ptr = (U64)old_ptr + sizeof(T);
//			U64 pers_mod = pers_ptr % alignment_of(T);
//			current = (void*)(pers_ptr + sizeof(void*) + ((pers_mod == 0) ? 0 : (alignment_of(T) - pers_mod))); //alignement
//
//			if (memory_full())
//				return nullptr;
//
//			*(void*)((U64)current - sizeof(void*)) = old_ptr;
//			return current;
//
//			void free_temp_top();
//
//			bool free(T* t){
//
//			}
//	private:
//		U64 size; //bytes
//		void *start, *current, *end;
//		StackAllocator::StackAllocator(){}
//		bool memory_full();
//		};
//
//
//	};
//}
