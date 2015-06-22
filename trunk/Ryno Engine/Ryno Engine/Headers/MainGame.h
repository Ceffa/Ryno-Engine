#pragma once
#include "MainGameInterface.h"

namespace Ryno{
	class MainGame : public MainGameInterface
	{
	private:
	
		void start() override;
		void input() override;
		void update() override;
		void draw() override;



		bool swap_curve = false;
		I64 speed = 40;
		I64 time = 0;;
		std::vector<GameObject*> m_game_objects;
		

	};
}
