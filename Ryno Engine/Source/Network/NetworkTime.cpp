#include "NetworkTime.h"
#include "TimeManager.h"
namespace Ryno {

	void NetworkTime::recv_time(F32 new_net_time)
	{
		F32 rtt = TimeManager::time - last_sent;
		time_samples[sample_index] = new_net_time + rtt / 2;
		time_recv[sample_index] = TimeManager::time;
		sample_index = ++sample_index %TIME_SAMPLES;

	}

	void NetworkTime::update_time() {
		net_time = 0;
		for (I32 i = 0; i < TIME_SAMPLES; i++) {
			net_time += time_samples[i] + (TimeManager::time -  time_recv[i]);
		}
		net_time /= TIME_SAMPLES;
	}

	F32 NetworkTime::get_time() const {
		return net_time;
	}
}