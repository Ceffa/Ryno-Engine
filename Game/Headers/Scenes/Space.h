#pragma once
#include "NetworkScene.h"
#define MAX_CLIENTS 30
#define NO_SCORE 9999999
namespace Ryno{
	enum ObjectCode {PLAYER,BALL};
	class Space : public NetworkScene
	{
	friend class Client;

	public:
		Space() : NetworkScene(){}
	private:
	
		void start() override;
		void update() override;
		void input() override;
		void on_network_recv(const NetMessage* message) override;
		void on_network_send(NetObject* sender, NetMessage* message) override;
		void on_periodic_update_send(NetMessage* message) override;
		void on_periodic_update_recv(NetMessage* message) override;
		void on_client_started() override;
		void receive_player(const NetMessage* message);
		void receive_ball(const NetMessage* message);

		ColorRGBA get_start_color_from_id(U32 i);
		glm::vec3 get_start_pos_from_id(U32 i);

		void initialize_player(const NetObject* net_obj);
		void initialize_ball(const NetObject* net_obj);

		GameObject cube;
		NetObject* controlled = nullptr;
		NetObject* ball = nullptr;
		std::list<NetObject*> players;
		I32 ship_mesh, sphere_mesh;
		Shader shader, material;
		Texture white, white_normal;
		GUIObject timer,score_text, client_text, hint;
		Font font;

		U32 scores[MAX_CLIENTS];
	};
}
