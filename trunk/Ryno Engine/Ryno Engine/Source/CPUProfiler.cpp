#include "GPUProfiler.h"
#include <gl\glew.h>


namespace Ryno{

	std::vector<GPUProfiler::Times> GPUProfiler::times;
	U8 GPUProfiler::current_time;

	void GPUProfiler::begin(){
		times.resize(0);
		current_time = 0;
	}
	void GPUProfiler::start_time(){
		times.emplace_back();
		glGenQueries(2, &times[current_time].query_id_0);
		glQueryCounter(times[current_time].query_id_0, GL_TIMESTAMP);
	}

	void GPUProfiler::next_time(){
		end_time();
		start_time();

	}

	void GPUProfiler::end_time(){
		glQueryCounter(times[current_time].query_id_1, GL_TIMESTAMP);
		current_time++;
	}

	void GPUProfiler::print_time(){


		for (U8 i = 0; i < current_time; i++){
			I32 stopTimerAvailable = 0;
			while (!stopTimerAvailable) {
				glGetQueryObjectiv(times[i].query_id_1,
					GL_QUERY_RESULT_AVAILABLE,
					&stopTimerAvailable);
			}

			// get query results
			glGetQueryObjectui64v(times[i].query_id_0, GL_QUERY_RESULT, &times[i].start);
			glGetQueryObjectui64v(times[i].query_id_1, GL_QUERY_RESULT, &times[i].end);

			printf("%f  ", (times[i].end - times[i].start) / 1000000.0);
		}
		printf("\n");
	}
}