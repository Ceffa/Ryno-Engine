#pragma once

#include "Space.h"
namespace Ryno {

	void Space::start() {

		client = Network::get_instance()->client;

		camera->position = glm::vec4(0, 20, 0, 1);
		camera->yaw = 0;
		camera->pitch = 90;
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
		

		if (!game->shell->active) {
			if (controlled) {
				float speed = 10.0f;
				float scaleSpeed = 5.0f;
				float rotSpeed = 70.0f;
				if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time* speed * glm::vec3(1, 0, 0));
				}
				if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time * speed * glm::vec3(-1, 0, 0));
				}
				if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time * speed * glm::vec3(0, 0, 1));
				}
				if (game->input_manager->is_key_down(SDLK_DOWN, KEYBOARD)) {
					controlled->game_object->transform.add_position(TimeManager::delta_time * speed * glm::vec3(0, 0, -1));
				}
				if (game->input_manager->is_key_down(SDLK_z, KEYBOARD)) {
					controlled->game_object->transform.add_scale(TimeManager::delta_time * scaleSpeed * glm::vec3(1,1,1));
				}
				if (game->input_manager->is_key_down(SDLK_x, KEYBOARD)) {
					controlled->game_object->transform.add_scale(TimeManager::delta_time * scaleSpeed * glm::vec3(-1, -1, -1));
				}
				if (game->input_manager->is_key_down(SDLK_n, KEYBOARD)) {
					controlled->game_object->transform.add_rotation(TimeManager::delta_time * rotSpeed * glm::vec3(0,1,0));
				}
				if (game->input_manager->is_key_down(SDLK_m, KEYBOARD)) {
					controlled->game_object->transform.add_rotation(TimeManager::delta_time * rotSpeed * glm::vec3(0,-1,0));
				}
			}
		}
	}
	void Space::on_network_recv(const NetMessage* message) {
		if (message->object_update.code == ObjectCode::PLAYER)
			receive_player(message);
	}

	void Space::receive_player(const NetMessage* message) {
	
		NetObject* received = NetObject::find(message->header.id);

		glm::vec3 pos = message->object_update.get_pos();
		glm::quat rot = message->object_update.get_rot();
		glm::vec3 scale = message->object_update.get_scale();

	

		//Reset position if just intantiated, otherwise just set the network pos
		if (received == nullptr) {
			received = create_net_obj(message->header.id);
			initialize_net_obj(received);
			received->reset_network_transform(pos,rot,scale);
			received->game_object->get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", get_start_color_from_id(message->header.id.client_id));

		}
		else
			received->set_network_transform(pos, rot, scale);

		received->last_modified = client->net_time.get_time();
	}

	void Space::on_network_send(NetObject* sender, NetMessage* message) {
		message->header.id = sender->id;
		message->header.code = NetCode::OBJECT;
		message->object_update.set_pos(sender->game_object->transform.get_position());
		message->object_update.set_rot(sender->game_object->transform.get_rotation());
		message->object_update.set_scale(sender->game_object->transform.get_scale());
		message->object_update.code = ObjectCode::PLAYER;
	}

	void Space::on_client_started() {
		if (!controlled) {
			controlled = create_net_obj(NetId(client->client_id));
			initialize_net_obj(controlled);
			
		
			controlled->reset_network_transform(get_start_pos_from_id(client->client_id),glm::quat(glm::vec3(0,0,0)),glm::vec3(1,1,1));
			controlled->game_object->get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", get_start_color_from_id(client->client_id));
		}
	}

	void Space::initialize_net_obj(const NetObject* net_obj) {

		//net_obj->game_object->transform.set_scale(glm::vec3(1, 1, 1));
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

	ColorRGBA Space::get_start_color_from_id(U32 i) {
		ColorRGBA c = ColorRGBA::black;
		if (i == 0 || i == 4 || i == 6 || i == 7)
			c.r = 255;
		if (i == 1 || i == 4 || i == 5 || i == 7)
			c.g = 255;
		if (i == 2 || i == 5 || i == 6 || i == 7)
			c.b = 255;
		return c;
	}
	glm::vec3 Space::get_start_pos_from_id(U32 i) {
		static F32 off = 5;
		glm::vec3 p = glm::vec3(0, 0, 0);
		if (i < 3)
			p.z = off;
		else if (i>4)
			p.z = -off;

		if (i == 0 || i == 3 || i == 5)
			p.x = -off;
		else if (i == 2 || i == 4 || i == 7)
			p.x = off;
		return p;
	}

}