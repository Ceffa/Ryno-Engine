#pragma once

#include "Types.h"
#include "Transform.h"
#include "GameObject.h"

namespace Ryno {

	class Script {
		friend class Scene;
		friend class Game;
	public:
		Script() { start(); }
		virtual ~Script() {}

		virtual void start() = 0;
		virtual void update() = 0;

		GameObject* gameObject;
	};

}
