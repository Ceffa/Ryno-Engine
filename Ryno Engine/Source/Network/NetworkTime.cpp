#include "NetworkTime.h"
#include "TimeManager.h"
namespace Ryno {

	void NetworkTime::recv_time(F32 new_net_time)
	{
		F32 rtt = TimeManager::time - last_sent;
		time_samples[sample_index] = new_net_time + rtt / 2;
		sample_index = ++sample_index %TIME_SAMPLES;

		for (I32 i = 0; i < TIME_SAMPLES; i++) {
			network_time += time_samples[i];
		}
		network_time /= TIME_SAMPLES;
	}

	void NetworkTime::send_time(F32 new_client_time)
	{
		last_sent = new_client_time;
	}

}