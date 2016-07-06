#pragma once

#include "PhysicsScene.h"
#include "Collider.h"

namespace Ryno {



	void PhysicsScene::start() {
		AABBCollider c;
		c.size = glm::vec3(12, 12, 12);
		Log::print(c.size.x);
	}

	void PhysicsScene::update() {
	
	}

	void PhysicsScene::input() {
	}
}