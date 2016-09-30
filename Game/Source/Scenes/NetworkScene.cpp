#pragma once

#include "NetworkScene.h"
#include "Network.h"
#include <thread>
namespace Ryno {

	void NetworkScene::start() {
		Network::get_instance()->init();
	}

	void NetworkScene::update() {

	}
	
	
}