#include "GPUProfiler.h"
#include <gl\glew.h>


namespace Ryno{

	U32 GPUProfiler::queries[2]{ {0} };

	void GPUProfiler::start_time(){
		glGenQueries(2, queries);
		glQueryCounter(queries[0], GL_TIMESTAMP);
	}

	float GPUProfiler::get_time(){
		glQueryCounter(queries[1], GL_TIMESTAMP);
		I32 stopTimerAvailable = 0;
		while (!stopTimerAvailable) {
			glGetQueryObjectiv(queries[1],
				GL_QUERY_RESULT_AVAILABLE,
				&stopTimerAvailable);
		}

		U64 start, end;
		glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &start);
		glGetQueryObjectui64v(queries[1], GL_QUERY_RESULT, &end);
		return ((float)(end - start)) / 1000000.0;
	}


}