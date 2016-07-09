#include "CPUProfiler.h"
#include <SDL\SDL.h>


namespace RynoEngine{

	std::vector<CPUProfiler::Times> CPUProfiler::times;
	U8 CPUProfiler::current_time;

	void CPUProfiler::begin(){
		times.clear();
		current_time = 0;
	}
	void CPUProfiler::start_time(){
		times.emplace_back((U32)SDL_GetTicks());
	}

	void CPUProfiler::next_time(){
		end_time();
		start_time();

	}

	void CPUProfiler::end_time(){
		times[current_time].end = (U32) SDL_GetTicks();
		current_time++;
	}

	void CPUProfiler::print_time(){


		for (U8 i = 0; i < current_time; i++){

			printf("%f  ", (F32)times[i].get_elapsed_time());

		}
		printf("\n");
	}

	void CPUProfiler::reset()
	{
		times.clear();
	}

}