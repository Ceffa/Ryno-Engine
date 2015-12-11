#pragma once
#include "Global.h"
#include <unordered_map>

namespace Ryno{

	class Game;

	class Scene{
	public:

		Scene(std::string name);
		virtual void start() = 0;
		virtual void input() = 0;
		virtual void update() = 0;

		static std::map<std::string, Scene*> scenes;

	protected:
		Game* game;



	};


}

