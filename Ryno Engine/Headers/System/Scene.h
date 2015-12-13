#pragma once
#include "Global.h"

namespace Ryno{

	class Game;

	class Scene{
	public:
		Camera3D* camera;

		Scene();
		virtual ~Scene() = 0{}
		virtual void start() = 0;
		virtual void input() = 0;
		virtual void update() = 0;
		void camera_update();
	protected:
		Game* game;



	};


}

