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
		virtual bool is_unique() { return false; }
		virtual bool is_copyable() { return true; }

		template<class T>
		static bool is_type(Script* s) {
			if (dynamic_cast<T*>(s))
				return true;
			return false;
		}

	};

}
