#pragma once
#include "NetStructs.h"

#define TIME_SAMPLES 4

namespace Ryno{
	
	//Class that syncrhonize network time across clients.
	//It keeps a list of the last times received from the server,
	//and average them to avoid spikes
	class NetworkTime {
	friend class Client;
	public:
		NetworkTime() : update_frequence(1),last_sent(-999) {}
		NetworkTime(F32 _update_frequence) : update_frequence(_update_frequence) {}
		
		//Update and getter of net time
		void update_time();
		F32 get_time() const;

	private:
		F32 last_sent;					//local time of the last sent client time, used to get rtt

		void recv_time(F32 new_net_time);
		F32 time_recv[TIME_SAMPLES];	//Keep track of WHEN the last times were received from server
		F32 time_samples[TIME_SAMPLES];	//Keep track of the last times received from server
		F32 update_frequence;			//How frequently request a new time to the server			
		U32 sample_index;			
		F32 net_time;					//last calculated net time

	};
}