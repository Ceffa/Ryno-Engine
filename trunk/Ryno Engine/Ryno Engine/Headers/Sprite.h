#pragma once
#include "Global.h"
#include "Structures.h"
#include <GLM/glm.hpp>
#include <list>

enum AnchorPoint{TOP_LEFT,TOP_RIGHT,BOTTOM_LEFT, BOTTOM_RIGHT, CENTER};

namespace Ryno{

	class Sprite{
	public:
		Sprite();
		~Sprite();
		Sprite(const Sprite *s);

		void generate_model_matrix();
		U32 get_texture_id() const { return texture.id; };
		void set_texture(Texture t);
		void set_ratio(U32 w, U32 h);
		void set_default_scale();

		//Sprites data
		glm::mat3 model_matrix;
		glm::vec2 position;
		glm::vec2 scale;
		glm::vec2 ratio;
		ColorRGBA color;
		glm::vec2 tiling;
		AnchorPoint anchor_point = TOP_LEFT;
		F32 angle;
		
		//static list of sprites
		static std::list<Sprite*> sprites;

	private:
		Texture texture;

	};


}