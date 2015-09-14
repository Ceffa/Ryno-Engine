#pragma once
#include "Global.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "DeferredRenderer.h"
#include "Sprite.h"
#include "Text.h"
#include <list>
#include <iterator>


namespace Ryno{
	class IConsole{
		
	public:
		~IConsole(){}
		void init();
		virtual void show() = 0;
		virtual void hide() = 0;
		virtual void toggle() = 0;
		void set_text_color(U8 r, U8 g, U8 b);
	
		bool active = true;
		Sprite* background;

	protected:
		IConsole(){}

		InputManager* input_manager;
		Font* font;

		std::vector<Text*> lines;
		std::list<std::string> history;
		std::list<std::string>::iterator iterator;
		U32 history_length;

		virtual void set(bool b) = 0;
	};

}