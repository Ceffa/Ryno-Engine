#pragma once

#include "WaveScene.h"

namespace Ryno {

	void WaveScene::start() {

		Batch3DGeometry::sorting = false;
		Batch3DShadow::sorting = false;
		camera->position = glm::vec4(-90,72,-294, 1);
		camera->yaw = 0.308f;
		camera->pitch = 0.106;
		camera->movement_speed = 100;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("day", ".png", GAME);


		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		mesh = game->mesh_manager->load_mesh("cube", GAME);
		sphere = game->mesh_manager->load_mesh("sphere", GAME);

		shader.create("Geometry/geometry", GAME);
		dir_light_shader.create("LightPass/directional", ENGINE);
		point_light_shader.create("LightPass/point", ENGINE);



		t.transform.set_scale(size/2,size/2,size/2);
		auto& m = t.add_component<Model>()->add_sub_model();


		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = mesh;
		m.cast_shadows = false;
		

		

		cubes.resize(lato * lato);
		for (I32 i = -lato/2; i < lato/2; i++) {
			for (I32 j = -lato/2; j < lato/2; j++) {
				cubes[(i+lato/2) * lato + j+lato/2].copy(t);
				cubes[(i + lato / 2) * lato + j + lato / 2].transform.set_position(i*size, 0, j*size);
			}
		}

		//dir light
		auto* l = t.add_component<DirectionalLight>();
		l->model.material.set_shader(&dir_light_shader);
		l->set_rotation(-50, 10, 0);
		l->diffuse_intensity = 1.5f;
		l->set_diffuse_color(255, 235, 200);
		l->specular_intensity = 20;
		l->set_specular_color(0, 0, 0);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 255);
		l->shadows = false;

	
		t.delete_component<Model>();
	

		
	}

	void WaveScene::update() {
		
		F32 _time = TimeManager::time * 1000;
		for (GameObject& o : cubes) {
			o.transform.set_position(o.transform.get_position().x, 15.0f*sin(sqrt(pow(o.transform.get_position().x / 25.0f, 2) + pow(o.transform.get_position().z / 25.0f, 2)) - (_time / 400.0f)), o.transform.get_position().z);
			
			o.get_component<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", ColorRGBA(0, (o.transform.get_position().y + 15.0f)*255.0f / 30.0f, 255 - (o.transform.get_position().y + 15.0f)*255.0f / 30.0f, 150));
		}
	}

	
}