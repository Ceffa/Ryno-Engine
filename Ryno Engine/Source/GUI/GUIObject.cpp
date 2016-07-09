#include "GUIObject.h"

namespace RynoEngine{

	
	std::list<GUIObject*> GUIObject::gui_objects;

	GUIObject::GUIObject(){
		gui_objects.push_back(this);
		sprite = nullptr;
		text = nullptr;
	}
	GUIObject::GUIObject(const GUIObject& go) : GUIObject()
	{

		if (go.sprite)
			sprite = new Sprite(go.sprite);
		else
			sprite = nullptr;
		if (go.text)
			text = new Text(go.text);
		else text = nullptr;

	}
	GUIObject::~GUIObject(){
		if (this)
			gui_objects.remove(this);
	}

}

