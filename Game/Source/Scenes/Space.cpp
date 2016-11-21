#pragma once

#include "Space.h"
namespace Ryno {

	void Space::start() {

		client = Network::get_instance()->client;

		camera->position = glm::vec4(0, 0, -10, 1);
		camera->yaw = 0;
		camera->pitch = 0;
		camera->movement_speed = 20;
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

		font.create("inconsolata", 40, ENGINE);
		timer.text = new Text();
		timer.text->set_position(1,1);
		timer.text->set_scale(1, 1);
		timer.text->set_color(70,70,70,255);
		timer.text->anchor_point = AnchorPoint::TOP_RIGHT;
		timer.text->text = "Heya";
		timer.text->font = &font;

	}

	void Space::update() {
		timer.text->text = std::to_string(client->net_time.get_time());
	}

	void Space::input() {
		if (!controlled) {
			controlled = create_net_obj(NetId(client->local_address));
			initialize_net_obj(controlled);
			controlled->game_object->transform.set_position(ryno_math::rand_vec3_range(glm::vec3(-4, -2, -1), glm::vec3(4, 2, 1)));
			controlled->game_object->get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", ryno_math::rand_color_range(ColorRGBA::black, ColorRGBA::white));
		}

		if (!game->shell->active) {
			if (controlled) {
				float speed = 10.0f;
				if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time* speed * glm::vec3(1, 0, 0));
				}
				if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time * speed * glm::vec3(-1, 0, 0));
				}
				if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time * speed * glm::vec3(0, 1, 0));
				}
				if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time * speed * glm::vec3(0, -1, 0));
				}
			}
		}
	}
	void Space::network_recv(const NetMessage* message) {
		NetObject* received = NetObject::find(message->header.id);

		if (received == nullptr) {
			received = create_net_obj(message->header.id);
			initialize_net_obj(received);
			ColorRGBA color = NetStruct::convert<ColorRGBA>(message->pos_and_color.color);
			received->game_object->get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", color);
		}

		received->last_modified = client->net_time.get_time();

		F32 x = NetStruct::convert<F32>(message->pos_and_color.x);
		F32 y = NetStruct::convert<F32>(message->pos_and_color.y);
		F32 z = NetStruct::convert<F32>(message->pos_and_color.z);


		glm::vec3 p = glm::vec3(x, y, z);
		received->set_network_position(glm::vec3(x, y, z));
	}

	void Space::network_send(NetObject* sender, NetMessage* message) {
		message->header.id = sender->id;
		message->header.code = NetStruct::convert<U32>(NetCode::POS_AND_COL);
		glm::vec3 p = sender->game_object->transform.get_position();
		ColorRGBA col = *(ColorRGBA*)sender->game_object->get_script<Model>()->sub_models[0].material.get_attribute("in_DiffuseColor");

		message->pos_and_color.x = NetStruct::convert<U32>(p.x);
		message->pos_and_color.y = NetStruct::convert<U32>(p.y);
		message->pos_and_color.z = NetStruct::convert<U32>(p.z);
		message->pos_and_color.color = NetStruct::convert<U32>(col);
	}

	void Space::initialize_net_obj(const NetObject* net_obj) {

		net_obj->game_object->transform.set_scale(glm::vec3(1, 1, 1));
		auto& m = net_obj->game_object->add_script<Model>()->add_sub_model();

		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = mesh;
		m.cast_shadows = false;
	}
}