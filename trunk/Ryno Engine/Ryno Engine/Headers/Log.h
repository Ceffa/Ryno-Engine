#pragma once
#include "Global.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "DeferredRenderer.h"
#include "Sprite.h"
#include "Text.h"
#include "IConsole.h"
#include <list>


#define NUM_LINES 15
#define HISTORY_LENGTH 50

namespace Ryno{
	class Log : public IConsole{
		friend class Shell;
	public:
		
		~Log(){}
		static Log* get_instance();
		void init();
		void show() override;
		void hide() override;
		void toggle() override;
		static void message(const std::string& message);
		static void message(F32 f);

		



	
	private:
		Log(){}
		static Log instance;
		void set(bool b) override;
		void refresh();
		void read_up();
		void read_down();
		void read_beginning();
		void read_end();

		


	};

}