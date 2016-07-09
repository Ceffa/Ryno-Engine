#pragma once

#include "Script.h"

namespace RynoEngine {

	class GroundElements : public Script {

	public:

		std::vector<GameObject> poles;
		Texture bt, bn;
		I32  cube_mesh;
		Game* game;
		Shader shader;
		GameObject base;


		GroundElements* clone() override {
			return new GroundElements(*this);
		}

		bool is_unique() override { return true; };
		bool is_copyable() override { return false; };

		void start() override {

		

			game = Game::get_instance();
			bt = game->texture_manager->load_png("pack/161.png", GAME);
			bn = game->texture_manager->load_png("pack/161_norm.png", GAME);

			cube_mesh = game->mesh_manager->load_mesh("cube", GAME);
			shader.create("Geometry/geometry", GAME);

			auto& sm = base.add_script<Model>()->add_sub_model();

			sm.material.set_shader(&shader);
			sm.mesh = cube_mesh;
			sm.cast_shadows = false;
			sm.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 0));
			sm.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

			sm.material.set_attribute("in_Tiling", glm::vec2(10, 10));
			sm.material.set_uniform("texture_sampler", bt.id);
			sm.material.set_uniform("normal_map_sampler", bn.id);

			base.transform.set_scale(200, 1, 200);
			base.transform.set_position(0, 0, 0);

			I32 s = 18;//size
			poles.resize(s*s);
			cube_mesh = game->mesh_manager->load_mesh("cube", GAME);

			auto& m = poles[0].add_script<Model>()->add_sub_model();
			m.material.set_shader(&shader);
			m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 0));
			sm.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

			m.material.set_attribute("in_Tiling", glm::vec2(.5f, 5));
			m.material.set_uniform("texture_sampler", bt.id);
			m.material.set_uniform("normal_map_sampler", bn.id);
			m.mesh = cube_mesh;
			m.cast_shadows = true;

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
		

	};

}
