#pragma once

#include "Types.h"


namespace Ryno {
	
	class GameObject;
	class Component {

	public:

		virtual ~Component() {}

		virtual Component* clone() = 0;

		virtual void start() {}
		virtual void update() {}
		virtual void input() {}
		GameObject* game_object;
		virtual bool is_unique() { return false; }
		virtual bool is_copyable() { return true; }

		template<class T>
		static bool is_type(Component* s) {
			if (dynamic_cast<T*>(s))
				return true;
			return false;
		}

	};

}
