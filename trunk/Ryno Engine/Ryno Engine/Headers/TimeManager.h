#pragma once
#include "Global.h"

namespace Ryno{

	class TimeManager{
	public:
		TimeManager();
		void init(F32 max_fps);
		void set_max_fps(F32 max_fps);
		void begin_frame();
		F32 end_frame();
		void print_fps();
	private:
		void calculate_FPS();
		F32 current_fps;
		F32 target_fps;
		U32 frame_time;
		U32 initial_ticks;
	};

}
