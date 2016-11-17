#pragma once
#include "NetStructs.h"

#define TIME_SAMPLES 4

namespace Ryno{
	
	class NetworkTime {
	public:
		F32 network_time;
		void recv_time(F32 new_net_time);
		void send_time(F32 new_client_time);

	private:
		F32 last_sent;					//CLIENT time of the last sent client time, used to get rtt

		F32 time_samples[TIME_SAMPLES];
		U32 sample_index;
	};
}