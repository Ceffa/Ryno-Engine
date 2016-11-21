#pragma once
#include "NetworkScene.h"

namespace Ryno{

	class Space : public NetworkScene
	{
	friend class Client;

	public:
		Space() : NetworkScene(){}
	private:
	
		void start() override;
		void update() override;
		void input() override;
		void network_recv(const NetMessage* message) override;
		void network_send(NetObject* sender, NetMessage* message) override;

		void initialize_net_obj(const NetObject* net_obj);

		GameObject cube;
		NetObject* controlled = nullptr;
		I32 mesh;
		Shader shader, dir_light_shader;
		Texture white, white_normal;
		GUIObject timer;
		Font font;
	};
}
