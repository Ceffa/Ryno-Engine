#include "CPUProfiler.h"
#include <gl\glew.h>
#include <iostream>


namespace Ryno {

	I64 CPUProfiler::time_0 = 0;

	void CPUProfiler::start_time() {
		glGetInteger64v(GL_TIMESTAMP, &time_0);
	}

	float CPUProfiler::get_time() {
		I64 end;
		glGetInteger64v(GL_TIMESTAMP, &end);

		return ((float)(end - time_0)) / 1000000.0;
	}

	void CPUProfiler::cout_time() {
		std::cout << get_time() << std::endl;
	}


}