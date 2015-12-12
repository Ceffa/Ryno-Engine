#pragma once
#include "Global.h"
#include "Structures.h"
#include <GLM/glm.hpp>
#include <list>


namespace Ryno{

	class Transform2D{
	public:
		virtual ~Transform2D(){}

		void set_position(F32 x, F32 y){ position.x = x; position.y = y; }
		void set_position(glm::vec2 pos){ position = pos; }
		void set_scale(F32 w, F32 h){ scale.x = w; scale.y = h; }
		void set_scale(glm::vec2 sc){ scale = sc; }
		glm::vec2 get_position(){ return position; }
		glm::vec2 get_scale(){ return scale; }
		
	protected:
		glm::vec2 position;
		glm::vec2 scale;

	};


}