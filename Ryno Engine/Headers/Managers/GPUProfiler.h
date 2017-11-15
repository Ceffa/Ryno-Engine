#pragma once
#include "Global.h"
#include <vector>

namespace Ryno{
	class GPUProfiler{
	private:
		static U32 queries[2];
	public:
		static void start_time();
		static float get_time();
		static void cout_time();

	};
}