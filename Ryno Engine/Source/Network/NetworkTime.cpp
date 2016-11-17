#include "NetworkTime.h"
#include "TimeManager.h"
namespace Ryno {

	void NetworkTime::recv_time(F32 new_net_time)
	{
		F32 rtt = TimeManager::time - last_sent;
		time_samples[sample_index] = new_net_time + rtt / 2;
		sample_index = ++sample_index %TIME_SAMPLES;
		time = 0;
		for (I32 i = 0; i < TIME_SAMPLES; i++) {
			time += time_samples[i];
		}
		time /= TIME_SAMPLES;
	}

}