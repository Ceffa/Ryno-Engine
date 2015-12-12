#include "GUIObject.h"

namespace Ryno{

	
	std::list<GUIObject*> GUIObject::gui_objects;

	GUIObject::GUIObject(){
		gui_objects.push_back(this);
	}
	GUIObject::GUIObject(const GUIObject* go) 
	{
		*this = *go;

		if (*go->sprite)
			sprite = new Sprite(*go->sprite);
		if (*go->text)
			text = new Text(*go->text);
		
		gui_objects.push_back(this);


	}
	GUIObject::~GUIObject(){
		gui_objects.remove(this);
	}

}

