#pragma once
#include "Global.h"
#include "Sprite.h"
#include "Text.h"	
#include <list>

namespace RynoEngine{

	class GUIObject{

	public:
		
		GUIObject();
		~GUIObject();

		GUIObject(const GUIObject& go);

		//Status
		bool active = true;

		//Components
		Text* text;
		Sprite* sprite;
	
		//Static GUIObjects List
		static std::list<GUIObject*> gui_objects;

	};


}
