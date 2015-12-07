#pragma once
#include "Types.h"
#include <iostream>
#include <initializer_list>
#include <unordered_map>

namespace Ryno{

	
	class ReferenceAllocator{
	public:
		static ReferenceAllocator* get_instance();

		bool init();

		//Allocate temp object
		template<typename... Args>
		void* temp_alloc(size_t obj_size, Args...args){
			void* ptr = malloc(obj_size);
			if (ptr == nullptr){
				std::cout << "Temp Reference Allocator: Malloc Failed." << std::endl;
				exit(-1);
			}
			entries[ptr] = true;
			return ptr;
		}


		//Allocate pers object
		template<typename... Args>
		void* pers_alloc(size_t obj_size, Args...args){
			void* ptr = malloc(obj_size);
			if (ptr == nullptr){
				std::cout << "Temp Reference Allocator: Malloc Failed." << std::endl;
				exit(-1);
			}
			entries[ptr] = false;
			return ptr;
		}

	private:

		ReferenceAllocator::ReferenceAllocator(){}

		

		//Free an object
		template<typename T, typename... Args>
		void free(T* t){
			entries.erase((void*)t);
			free(t);
		}

		//Free all temp objects
		void reset(); 


	private:
		std::unordered_map<void*, bool> entries;

	};
}