#pragma once
#include "Global.h"
#include <vector>

namespace Ryno{
	class CPUProfiler{
		struct Times{
			Times(U32 _start) :start(_start){}
			U32 get_elapsed_time(){
				return end - start;
			}
			U32 start;
			U32 end;
			};
	public:
		//n is the number of samples, helps to reallocate before.
		static void begin();
		static void start_time();
		//calls start_time and end_time.
		//useful to track multiple times one after the other
		static void next_time(); 
		static void end_time();
		static void print_time();

	private:
		static std::vector<Times> times;
		static U8 current_time;
	};
}