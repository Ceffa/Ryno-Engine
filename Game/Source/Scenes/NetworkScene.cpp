#pragma once

#include "NetworkScene.h"
#include <thread>
namespace Ryno {

	void NetworkScene::start() {

		net_entity = Network::get_instance()->net_entity;

		camera->position = glm::vec4(0,0,-10,1);
		camera->yaw = 0;
		camera->pitch = 0;
		camera->movement_speed = 100;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("day", ".png", GAME);

		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		mesh = game->mesh_manager->load_mesh("cube", GAME);

		shader.create("Geometry/geometry", GAME);
		dir_light_shader.create("LightPass/directional", ENGINE);

		

		//dir light
		auto* l = cube.add_script<DirectionalLight>();
		l->model.material.set_shader(&dir_light_shader);
		l->set_rotation(-50, 10, 0);
		l->diffuse_intensity = 1.5f;
		l->set_diffuse_color(255, 235, 200);
		l->specular_intensity = 20;
		l->set_specular_color(0, 0, 0);
		l->ambient_intensity = 0;
		l->set_ambient_color(255, 255, 255);
		l->shadows = false;

		controlled = create_net_obj(NetId(net_entity->local_address))->game_object;
		controlled->get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", ColorRGBA(255, 0, 0, 255));

	}

	void NetworkScene::input() {
		if (!game->shell->active) {

			float speed = 10.0f;
			if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
				controlled->transform.add_position(game->delta_time * speed * glm::vec3(1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
				controlled->transform.add_position(game->delta_time * speed * glm::vec3(-1, 0, 0));
			}
			if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
				controlled->transform.add_position(game->delta_time * speed * glm::vec3(0, 1, 0));
			}
			if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
				controlled->transform.add_position(game->delta_time * speed * glm::vec3(0, -1, 0));
			}
		}
	}
	void NetworkScene::network_object_created(const Message& message) {
		NetObject* received = NetObject::find(message.id);
		NetUtil::print(message.id.to_string());

		if(received == nullptr)
			received = create_net_obj(message.id);
		
		F32 x = *(F32*)&message.x;
		F32 y = *(F32*)&message.y;
		F32 z = *(F32*)&message.z;

		received->game_object->transform.set_position(glm::vec3(x,y,z));
		
	}

	NetObject* NetworkScene::create_net_obj(const NetId& id) {
		net_cubes.emplace_back();

		GameObject& cube = net_cubes.back();
		NetObject* net_obj = cube.add_script(new NetObject(id));
		cube.transform.set_scale(1,1,1);
		auto& m = cube.add_script<Model>()->add_sub_model();

		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = mesh;
		m.cast_shadows = false;

		return net_obj;
	}
}