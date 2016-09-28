#pragma once

#include "NetworkScene.h"
#include "GameObject.h"
#include "Particle.h"
#include "InertiaTensorGenerator.h"
#include <thread>
namespace Ryno {

	void NetworkScene::start() {
		t.create(&NetworkScene::try_network, this);
	}

	void NetworkScene::update() {

	}
	void NetworkScene::try_network() {
		Network::init();
		server.init();
		server.bind();
		server.listen();
		server.accept();
	}
	
}