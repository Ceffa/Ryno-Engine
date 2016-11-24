#pragma once

#include "Space.h"
namespace Ryno {

	void Space::start() {


		camera->position = glm::vec4(0, 20, 0, 1);
		camera->yaw = 0;
		camera->pitch = 90;
		camera->movement_speed = 20;
		camera->have_skybox = true;
		camera->skybox = game->texture_manager->load_cube_map("day", ".png", GAME);

		white = game->texture_manager->load_png("white_pixel.png", GAME);
		white_normal = game->texture_manager->load_png("normal_pixel.png", GAME);
		ship_mesh = game->mesh_manager->load_mesh("ship", GAME);
		sphere_mesh = game->mesh_manager->load_mesh("sphere", GAME);

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


		font.create("inconsolata", 26, ENGINE);
		timer.text = new Text();
		timer.text->set_position(1,0);
		timer.text->set_scale(2, 2);
		timer.text->set_color(0,0,0,255);
		timer.text->anchor_point = AnchorPoint::BOTTOM_RIGHT;
		timer.text->font = &font;
	
		score_text.text = new Text();
		score_text.text->set_position(1, 1);
		score_text.text->set_scale(2,2);
		score_text.text->set_color(0, 0, 0, 255);
		score_text.text->anchor_point = AnchorPoint::TOP_RIGHT;
		score_text.text->font = &font;

		client_text.text = new Text();
		client_text.text->set_position(0, 1);
		client_text.text->set_scale(2, 2);
		client_text.text->set_color(0, 0, 0, 255);
		client_text.text->anchor_point = AnchorPoint::TOP_LEFT;
		client_text.text->font = &font;

		for (I32 i = 0; i < MAX_CLIENTS; i++) {
			scores[i] = -1;
		}

	
	}

	void Space::update() {
		if (Network::has_client)
			timer.text->text = std::to_string(Network::client->net_time.get_time());
		
		if (Network::has_server) {

			for (auto& go : net_game_objects) {
				NetObject* no = go.get_script<NetObject>();
				if (no->tag == ObjectCode::PLAYER) {

					glm::vec3 dist = go.transform.get_position() - ball->game_object->transform.get_position();
					dist.y = 0;
					F32 squared_diff = glm::length2(dist);
					if (squared_diff < 3) {
						ball->last_sent = 0;	//require update to be sent immediately
						ball->reset_network_transform(ryno_math::rand_vec3_range(glm::vec3(-17, 0, -10), glm::vec3(17, 0, 10)), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(.4f), Network::client->net_time.get_time());
						scores[no->id.client_id]++;
						if (Network::has_server) {
							Network::server->last_periodic_update = -999;	//force update of clients
						}
					}

				}

			}

		}
	}

	void Space::input() {
		

		if (!game->shell->active) {
			if (controlled) {
				float speed = 20.0f;
				float scaleSpeed = .5f;
				float rotSpeed = 200.0f;

				if (game->input_manager->is_key_down(SDLK_UP, KEYBOARD)) {
					glm::vec3 dir = TimeManager::delta_time * speed * glm::vec3(-1,0,0);
					dir = glm::inverse(controlled->game_object->transform.get_rotation()) * dir;
					controlled->game_object->transform.add_position(dir);
				}

				if (game->input_manager->is_key_down(SDLK_z, KEYBOARD)) {
					controlled->game_object->transform.add_scale(TimeManager::delta_time * scaleSpeed * glm::vec3(1,1,1));
				}
				if (game->input_manager->is_key_down(SDLK_x, KEYBOARD)) {
					controlled->game_object->transform.add_scale(TimeManager::delta_time * scaleSpeed * glm::vec3(-1, -1, -1));
				}
				if (game->input_manager->is_key_down(SDLK_LEFT, KEYBOARD)) {
					controlled->game_object->transform.add_rotation(TimeManager::delta_time * rotSpeed * glm::vec3(0,1,0));
				}
				if (game->input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)) {
					controlled->game_object->transform.add_rotation(TimeManager::delta_time * rotSpeed * glm::vec3(0,-1,0));
				}
			}
		}
	}
	void Space::on_network_recv(const NetMessage* message) {
		if (message->object_update.code == ObjectCode::PLAYER)
			receive_player(message);
		else if (message->object_update.code == ObjectCode::BALL)
			receive_ball(message);


	}

	void Space::receive_player(const NetMessage* message) {
	
		NetObject* received = NetObject::find(message->header.id);

		glm::vec3 pos = message->object_update.get_pos();
		glm::quat rot = message->object_update.get_rot();
		glm::vec3 scale = message->object_update.get_scale();


		//Reset position if just intantiated, otherwise just set the network pos
		if (received == nullptr) {
			received = create_net_obj(message->header.id);
			scores[message->header.id.client_id] = 0;
			received->tag = ObjectCode::PLAYER;
			initialize_player(received);
			received->reset_network_transform(pos,rot,scale,message->header.get_timestamp());
			received->game_object->get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", get_start_color_from_id(message->header.id.client_id));
		}
		else
			received->set_network_transform(pos, rot, scale,message->header.get_timestamp());

		received->last_received = Network::client->net_time.get_time();
	}

	void Space::receive_ball(const NetMessage* message) {

		NetObject* received = NetObject::find(message->header.id);

		glm::vec3 pos = message->object_update.get_pos();
		glm::quat rot = message->object_update.get_rot();
		glm::vec3 scale = message->object_update.get_scale();


		//Reset position if just intantiated, otherwise just set the network pos
		if (received == nullptr) {
			received = create_net_obj(message->header.id);
			received->tag = ObjectCode::PLAYER;
			initialize_ball(received);
		}

		received->reset_network_transform(pos, rot, scale, message->header.get_timestamp());

		received->last_received = Network::client->net_time.get_time();
	}

	void Space::on_network_send(NetObject* sender, NetMessage* message) {
		message->header.id = sender->id;
		message->header.code = NetCode::OBJECT;
		message->object_update.set_pos(sender->game_object->transform.get_position());
		message->object_update.set_rot(sender->game_object->transform.get_rotation());
		message->object_update.set_scale(sender->game_object->transform.get_scale());
		message->object_update.code = sender == ball ? ObjectCode::BALL : ObjectCode::PLAYER;
	}
	void Space::on_periodic_update(NetMessage* message) {
		//If server send an update, if client read it
		if (Network::has_server) {
			I32 i = 0;
			for (i = 0; i < MAX_CLIENTS; i++) {
				if (scores[i] < 0) {
					if (i < Connection::last_client_id)
						scores[i] = 0;
					else
						break;
				}
				message->net_array.set_value(i, scores[i]);
			}
			message->net_array.length = i;
		}
		if (Network::has_client) {
			score_text.text->text = "";
						
			for (int i = 0; i < MAX_CLIENTS; i++) {
				I32 value = message->net_array.get_value(i);
				if (value < 0)
					break;
				std::string s;

				if (i == Network::client->client_id)
					s += "You ";
				else {
					s += "Ship ";
					s += std::to_string(i);
				}

				s += ": ";
				s += std::to_string(value);
				s += '\n';
				score_text.text->text += s;
			}
		}
	}


	void Space::on_client_started() {
		client_text.text->text = std::to_string(Network::client->client_id);
		if (!controlled) {
			controlled = create_net_obj(NetId(Network::client->client_id));
			scores[Network::client->client_id] = 0;
			controlled->tag = ObjectCode::PLAYER;
			initialize_player(controlled);
		
			controlled->reset_network_transform(get_start_pos_from_id(Network::client->client_id),glm::quat(glm::vec3(0,0,0)),glm::vec3(.2f,.15f,.1f), Network::client->net_time.get_time());
			controlled->game_object->get_script<Model>()->sub_models[0].material.set_attribute("in_DiffuseColor", get_start_color_from_id(Network::client->client_id));
		}

		if (!ball && Network::has_server) {
			ball = create_net_obj(NetId(Network::client->client_id));
			ball->tag = ObjectCode::BALL;
			initialize_ball(ball);
			ball->reset_network_transform(ryno_math::rand_vec3_range(glm::vec3(-17,4,-10),glm::vec3(17,4,10)),glm::quat(glm::vec3(0,0,0)),glm::vec3(.4f), Network::client->net_time.get_time());

		}
	}

	void Space::initialize_player(const NetObject* net_obj) {

		//net_obj->game_object->transform.set_scale(glm::vec3(1, 1, 1));
		auto& m = net_obj->game_object->add_script<Model>()->add_sub_model();

		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = ship_mesh;
		m.cast_shadows = false;

	}

	void Space::initialize_ball(const NetObject* net_obj) {

		//net_obj->game_object->transform.set_scale(glm::vec3(1, 1, 1));
		auto& m = net_obj->game_object->add_script<Model>()->add_sub_model();

		m.material.set_shader(&shader);
		m.material.set_attribute("in_DiffuseColor", ColorRGBA(255, 255, 255, 255));
		m.material.set_attribute("in_SpecularColor", ColorRGBA(255, 255, 255, 255));

		m.material.set_attribute("in_Tiling", glm::vec2(1, 1));
		m.material.set_uniform("texture_sampler", white.id);
		m.material.set_uniform("normal_map_sampler", white_normal.id);
		m.mesh = sphere_mesh;
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