#pragma once
#include "Global.h"

namespace Ryno{

	class TimeManager{
	public:
		static TimeManager* get_instance();
		void init(F32 max_fps);
		void set_max_fps(F32 max_fps);
		void begin_frame();
		void TimeManager::end_frame(F32* time, F32* delta_time);
		void print_fps();
		F32 slow_factor = 1.0f;

	private:
		TimeManager();
		void calculate_FPS();
		F32 current_fps;
		F32 target_fps;
		U32 frame_time;
		U32 initial_ticks;
	};

}
