#pragma once
#include "Global.h"
#include "Structures.h"
#include "ITransform2D.h"
#include <GLM/glm.hpp>
#include <list>


namespace Ryno{

	

	class Sprite : public ITransform2D{
	public:
		Sprite();
		~Sprite();
		Sprite(const Sprite *s);

		void generate_model_matrix();

		//Getter and setter
		void set_texture(Texture t);
		U32 get_texture_id() const { return texture.id; };
		Texture get_texture() const { return texture; }
		void set_tiling(F32 u, F32 v){ tiling.x = u; tiling.y = v; }
		glm::vec2 get_tiling(){ return tiling; }
		void set_color(U8 r, U8 g, U8 b, U8 a){ color.set_color_and_alpha(r, g, b, a); }
		ColorRGBA get_color(){ return color; }
	

		//Sprites data
		glm::mat3 model_matrix;
	
		U8 depth = 1;
		bool active = true;
		Use use = HUD;
		F32 ratio;
		AnchorPoint anchor_point = TOP_LEFT;
		F32 angle;
		
		//static list of sprites
		static std::list<Sprite*> sprites;

	private:
		Texture texture;
		glm::vec2 tiling;
		ColorRGBA color;

	};


}