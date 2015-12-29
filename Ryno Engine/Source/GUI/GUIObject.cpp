#include "GUIObject.h"

namespace Ryno{

	
	std::list<GUIObject*> GUIObject::gui_objects;

	GUIObject::GUIObject(){
		gui_objects.push_back(this);
	}
	GUIObject::GUIObject(const GUIObject& go) : GUIObject()
	{

		if (*go.sprite)
			sprite.copy(go.sprite);
		if (*go.text)
			text.copy(go.text);

	}
	GUIObject::~GUIObject(){
		if (this)
			gui_objects.remove(this);
	}

}

