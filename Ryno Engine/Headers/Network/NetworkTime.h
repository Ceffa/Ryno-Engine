#pragma once
#include "NetStructs.h"

#define TIME_SAMPLES 4

namespace Ryno{
	
	class NetworkTime {
	friend class Client;
	public:
		NetworkTime() : update_frequence(1) {}
		NetworkTime(F32 _update_frequence) : update_frequence(_update_frequence) {}
		F32 time;
		void recv_time(F32 new_net_time);

	private:
		F32 last_sent;					//CLIENT time of the last sent client time, used to get rtt

		F32 time_samples[TIME_SAMPLES];
		U32 sample_index;
		F32 update_frequence;

	};
}