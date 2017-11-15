#pragma once
#include "Global.h"
#include <vector>

namespace Ryno {
	class CPUProfiler {
	private:
		static I64 time_0;
	public:
		static void start_time();
		static float get_time();
		static void cout_time();

	};
}