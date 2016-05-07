#pragma once

#include "Script.h"

namespace Ryno {

	class GroundElements : public Script {

	public:

		std::vector<GameObject> poles;
		Texture bt, bn;
		I32  cube_mesh;
		Game* game;
		Shader shader;

		void start() override {
			game = Game::get_instance();
			bt = game->texture_manager->load_png("pack/161", GAME);
			bn = game->texture_manager->load_png("pack/161_norm", GAME);

			cube_mesh = game->mesh_manager->load_mesh("cube", 1, GAME);
			shader.create("Geometry/geometry", GAME);

			game_object->model = new Model();
			game_object->model->material.set_shader(&shader);
			game_object->model->mesh = cube_mesh;
			game_object->model->cast_shadows = false;
			game_object->model->material.set_attribute("in_Color", ColorRGBA(255, 255, 255, 0));
			game_object->model->material.set_attribute("in_Tiling", glm::vec2(10, 10));
			game_object->model->material.set_uniform("texture_sampler", bt.id);
			game_object->model->material.set_uniform("normal_map_sampler", bn.id);

			game_object->transform.set_scale(200, 1, 200);
			game_object->transform.set_position(0, 0, 0);

			I32 s = 18;//size
			poles.resize(s*s);
			cube_mesh = game->mesh_manager->load_mesh("cube", 1, GAME);

			poles[0].model = new Model();
			auto* m = poles[0].model;
			m->material.set_shader(&shader);
			m->material.set_attribute("in_Color", ColorRGBA(255, 255, 255, 0));
			m->material.set_attribute("in_Tiling", glm::vec2(.5f, 5));
			m->material.set_uniform("texture_sampler", bt.id);
			m->material.set_uniform("normal_map_sampler", bn.id);
			m->mesh = cube_mesh;
			m->cast_shadows = true;

			poles[0].transform.set_scale(2, 20, 2);
			poles[0].transform.set_position(0, 0, 0);
			for (I32 i = 0; i < s; i++)
			{
				for (I32 j = 0; j < s; j++)
				{
					if (i == j && i == 0)continue;
					poles[i*s + j].copy(poles[0]);
					poles[i*s + j].transform.set_position((i - s / 2) * 20, 10, (j - s / 2) * 20);
				}
			}

		}
		void input() override {
		
		}

		void update() override {
		}
	

	};

}
