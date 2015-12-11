////THIS CLASS IS NOT WORKING, TO BE FINISHED//
//
//#pragma once
//#include "Types.h"
//#include <iostream>
//#include <initializer_list>
//
//namespace Ryno{
//
//	//Keep track of the offsets of:
//	//-the new obj in the stack
//	//-the obj destructor relative to the base of the obj
//	class ObjPtr{
//	public:
//		template<typename T>
//		ObjPtr(T* t, void* prev_ptr, void* new_ptr){
//			previous_obj = (U32)((U64)new_ptr - (U64)prev_ptr);
//			destructor_offset = (U32)((U64)&t->~T - (U64)t);
//		}
//		U32 previous_obj;
//		U32 destructor_offset;
//
//	};
//	class StackAllocator{
//	public:
//		static StackAllocator* get_instance();
//
//		bool init(U64 MB_size);
//
//
//		template<typename T, typename... Args>
//		T* temp_alloc(T*t, Args...args){
//			U64 old_ptr = (U64)temp_current;
//			U64 temp_ptr = old_ptr - sizeof(T);
//			temp_current = (void*)(temp_ptr - (temp_ptr % sizeof(U64)) - sizeof(U64)) ; //alignement
//			
//			if (memory_full())
//				return nullptr;
//
//			*(ObjPtr*)temp_current = ObjPtr(t,(void*)old_ptr,(void*)temp_current);
//			T* final = (T*)((U64)temp_current + sizeof(U64));
//			new (final) T(args...);
//			return final;
//		}
//
//		template<typename T, typename... Args>
//		T* pers_alloc(T*t, Args...args){
//			U64 old_ptr = (U64)pers_current;
//			U64 pers_ptr = old_ptr + sizeof(T);
//			U64 pers_mod = pers_ptr % sizeof(U64);
//			pers_current = (void*)(pers_ptr + sizeof(U64) + ((pers_mod == 0) ? 0 : (sizeof(U64) - pers_mod))); //alignement
//			
//			if (memory_full())
//				return nullptr;
//
//			*(ObjPtr*)((U64)pers_current - sizeof(U64)) = ObjPtr(t,(void*)old_ptr,(void*)pers_current);
//			T* final = (T*)old_ptr;
//			new (final)T(args...);
//			return final;
//		}
//
//		void free_temp_top();
//
//	private:
//		U64 size; //bytes
//		void *pers_start, *pers_current, *temp_start, *temp_current;
//		StackAllocator::StackAllocator(){}
//		bool memory_full();
//	};
//
//
//}
