#pragma once
#include "Types.h"
#include <iostream>
#include <initializer_list>

namespace Ryno{

	class Pool{
	public:
		static Pool* get_instance();

		bool init();

	private:

		Pool::Pool(){}
	};
}