#pragma once

#include "Types.h"
#include "GameObject.h"
#include "Game.h"

namespace Ryno {

	class Script {
		friend class Scene;
		friend class Game;
		friend class GameObject;
		friend class Transform;
	public:
	
		virtual ~Script() {}

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void input() = 0;

		GameObject* gameObject;
	};

}
