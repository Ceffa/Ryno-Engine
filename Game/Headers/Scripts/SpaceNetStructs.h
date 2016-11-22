#pragma once

#include "Global.h"
#include "NetStructs.h"


namespace Ryno {

	//Sent by client periodically
	struct ClientUpdate : public NetStruct {
		void to_network_order() override {
		}
		void to_hardware_order() override {
		}
	};
	struct ServerUpdate : public NetStruct {
		U32 net_time;
		U32 client_id;
		void to_network_order() override {
			net_time = htonl(net_time);
			client_id = htonl(client_id);
		}
		void to_hardware_order() override {
			net_time = ntohl(net_time);
			client_id = ntohl(client_id);
		}
	};

	struct Player : public NetStruct {
		Player() {}
		Player(U32 _x, U32 _y, U32 _z, U32 _rot) :x(_x), y(_y), z(_z), rot(_rot) {}
		U32 x, y, z,rot;
		void to_network_order() override {
			x = htonl(x);
			y = htonl(y);
			z = htonl(z);
			rot = htonl(rot);
		}
		void to_hardware_order() override {
			x = ntohl(x);
			y = ntohl(y);
			z = ntohl(z);
			rot = htonl(rot);
		}
	};

	struct Ball : public NetStruct {
		Ball() {}
		Ball(U32 _x, U32 _y, U32 _z) :x(_x), y(_y), z(_z) {}
		U32 x, y, z, rot;
		void to_network_order() override {
			x = htonl(x);
			y = htonl(y);
			z = htonl(z);
		}
		void to_hardware_order() override {
			x = ntohl(x);
			y = ntohl(y);
			z = ntohl(z);
		}
	};

	enum NetCode { CLIENT_UPDATE, SERVER_UPDATE, PLAYER, BALL };

	struct SpaceMessage : public NetMessage {
		SpaceMessage() {}
		union {
			ClientUpdate client_update;
			ServerUpdate server_update;
			Player player;
			Ball ball;
			
		};
			};


}