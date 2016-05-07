#pragma once
#include "ValleyScene.h"
#include "FireworkScene.h"
#include "Terrain.h"
#define DEG_TO_RAD 0.0174532925199433

namespace Ryno {

	void ValleyScene::start() {

		camera->position = glm::vec4(-100, 50, -100, 1);
		camera->movement_speed = .1f;
		camera->skybox = game->texture_manager->load_cube_map("full_moon", GAME);

		sphere.add_script(&lights_script);
		floor.add_script(&ground_script);
	}

	void ValleyScene::update() {
	}

	void ValleyScene::input() {
	}
}