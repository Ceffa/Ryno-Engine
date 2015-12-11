#pragma once
#include "Global.h"

namespace Ryno{

	class Game;

	class Scene{
	public:

		Scene();
		virtual void start() = 0;
		virtual void input() = 0;
		virtual void update() = 0;

	protected:
		Game* game;



	};


}

