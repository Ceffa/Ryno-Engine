#include "TimeManager.h"
#include <SDL/SDL.h>
#include <iostream>

namespace Ryno{

	TimeManager::TimeManager() : current_fps(60){

	}
	void TimeManager::init(F32 max_fps){
		set_max_fps(max_fps);
	}
	void TimeManager::set_max_fps(F32 max_fps){
		target_fps = max_fps;

	}
	void TimeManager::begin_frame(){
		initial_ticks = SDL_GetTicks();
	}
	float TimeManager::end_frame(){

		calculate_FPS();

		//limit fps
		U32 time_passed = SDL_GetTicks() - initial_ticks;
		if ((U32)(1000.0f / target_fps) > time_passed) {
			SDL_Delay((U32)(1000.0f / target_fps) - time_passed);
		}
		return current_fps;

	}
	void TimeManager::print_fps(){
		static U32 frame_count = 0;
		if (frame_count++ == 30) {
			frame_count = 0;
			//std::cout << current_fps << std::endl;
		}
	}

	void TimeManager::calculate_FPS() {
		//Initial values
		static const U8 NUM_SAMPLES = 20;		
		static U32 frame_times[NUM_SAMPLES];
		static U64 current_frame = 0;
		static I32 prev_ticks = SDL_GetTicks();
		U32 current_ticks;
		
		//Get the time passed from the last frame, and set it as old for the next iteration
		current_ticks = SDL_GetTicks();
		frame_time = current_ticks - prev_ticks;
		prev_ticks = current_ticks;

		//Add it in the sample buffer
		frame_times[current_frame++ % NUM_SAMPLES] = frame_time;
		
		//number of elements to sample
		U64 count;
		count = current_frame < NUM_SAMPLES ? current_frame : NUM_SAMPLES;

		//get som of last NUM_SAMPLES fps
		F32 frame_average = 0;
		for (U8 i = 0; i < count; i++) {
			frame_average += frame_times[i];
		}

		//get fps average
		frame_average /= count;

		if (frame_average > 0)	//avoid 0 division
			current_fps = 1000.0f / frame_average;
		else
			current_fps = 60.0f; //it shouldn't happen


	}


}