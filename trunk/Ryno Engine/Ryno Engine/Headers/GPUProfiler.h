#pragma once
#include "Types.h"
#include <vector>

namespace Ryno{
	class GPUProfiler{
		struct Times{
			U32 get_elapsed_time(){
				return end - start;
			}
			U64 start;
			U64 end;
			U32 query_id_0;
			U32 query_id_1;
		};
	public:
		//n is the number of samples, helps to reallocate before.
		static void begin(U8 n);
		static void start_time();
		static void end_time();
		static void print_time();

	private:
		static std::vector<Times> times;
		static U8 current_time;
	};
}