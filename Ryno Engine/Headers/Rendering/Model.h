#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Global.h"
#include "TextureManager.h"
#include "Structures.h"
#include <list>

namespace Ryno{
		
	class Model{
	public:
		Model();
		~Model(){ models.remove(this); }
		Model(const Model *m);
		void set_color(U8 r, U8 g, U8 b);
		void set_alpha(U8 f);
		void set_color_and_flatness(U8 r, U8 g, U8 b, U8 f);
			
		void set_tiling(U32 u, U32 v);
		void set_texture_normal(Texture t, Texture n);

		Texture texture,normal_map;
	
		I32 mesh;
		glm::vec2 tiling;
		ColorRGBA color;  //Alpha is flatness
		bool cast_shadows = true;

	private:
		static std::list<Model*> models;
		static void reset();
	};
}