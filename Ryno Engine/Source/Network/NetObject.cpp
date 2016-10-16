#include "NetObject.h"

namespace Ryno {

	void NetObjId::to_network_order()
	{
		ip = htonl(ip);
		port = htonl(port);
		local_id = htonl(local_id);
	}

	void NetObjId::to_hardware_order()
	{
		ip = ntohl(ip);
		port = ntohl(port);
		local_id = ntohl(local_id);
	}

}