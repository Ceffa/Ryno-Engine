#pragma once
#include "ValleyScene.h"
#include "FireworkScene.h"
#include "Terrain.h"
#define DEG_TO_RAD 0.0174532925199433

namespace Ryno {

	void ValleyScene::start() {

		camera->position = glm::vec4(-70.85f,185.26f,-312.74f, 1);
		camera->yaw = 0.1995f;
		camera->pitch = 0.6505f;
		camera->movement_speed = 100;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("full_moon", ".png", GAME);



		sphere.add_script<LightsElements>();
		floor.add_script<GroundElements>();
	}

	void ValleyScene::update() {
	}

	void ValleyScene::input() {
	}
}