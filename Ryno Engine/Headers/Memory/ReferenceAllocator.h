#pragma once
#include "Global.h"
#include <iostream>
#include <initializer_list>
#include <unordered_map>

namespace Ryno{

	
	class ReferenceAllocator{
	public:
		static ReferenceAllocator* get_instance();

		bool init();

	private:

		ReferenceAllocator::ReferenceAllocator(){}

		//Allocate temp object
		template<typename T, typename... Args>
		bool temp_alloc(T** t, Args...args){
			*t = new T(args);
			entries[(void*)*t] = true;
		}

		//Allocate pers object
		template<typename T, typename... Args>
		bool pers_alloc(T** t, Args...args){
			*t = new T(args);
			entries[(void*)*t] = false;
		}

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