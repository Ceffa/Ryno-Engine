#pragma once

#include "NetworkScene.h"
#include "Network.h"

namespace Ryno {

	NetObject* NetworkScene::create_net_obj(const NetId& id) {

		net_game_objects.emplace_back();

		GameObject* go = &(net_game_objects.back());
		return go->add_script(new NetObject(id));
	}

	void NetworkScene::remove_unused_net_objects()
	{
		for (auto it = net_game_objects.begin(); it != net_game_objects.end(); )  //No increment
		{
			NetObject* no = it->get_script<NetObject>();
			if (no->mark_for_destruction)
				it = net_game_objects.erase(it);
			else
				it++;
		}
	}

}