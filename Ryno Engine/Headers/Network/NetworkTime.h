#pragma once
#include "NetStructs.h"

#define TIME_SAMPLES 4

namespace Ryno{
	
	class NetworkTime {
	public:
		F32 network_time;
		void new_time(F32 net_time);

	private:
		F32 last_sent;					//CLIENT time of the last sent client time, used to get rtt

		F32 time_samples[TIME_SAMPLES];
		F32 sample_index;


	};
}