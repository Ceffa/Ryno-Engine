#pragma once

#include "Types.h"


namespace Ryno {
	
	class GameObject;
	class Script {

	public:

		virtual ~Script() {}

		virtual Script* clone() = 0;

		virtual void start() {}
		virtual void update() {}
		virtual void input() {}
		GameObject* game_object;
		virtual bool is_unique() = 0;
		virtual bool is_copyable() = 0;

	};

}
