#pragma once

#include "PhysicsScene.h"
#include "Collider.h"
#include "GameObject.h"

namespace RynoEngine {


	void PhysicsScene::start() {
		AABBCollider* aabb = ball.add_script<AABBCollider>();
		aabb->size = glm::vec3(12, 12, 12);
		Log::print(ball.get_script<AABBCollider>()->size.x);
	}

	void PhysicsScene::update() {
	
	}

	void PhysicsScene::input() {
	}
}