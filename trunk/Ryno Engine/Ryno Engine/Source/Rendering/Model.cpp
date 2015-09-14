#include "Model.h"


namespace Ryno{


	Model::Model(){
		color.set_color_and_alpha(255,255,255,0);
		normal_map.id = -1;
		set_tiling(1, 1);
	}

	



	Model::Model(const Model *m)
	{
		
		*this = *m;
	}

	void Model::set_color(U8 r, U8 g, U8 b){
		color.set_color(r, g, b);
	}
	void Model::set_alpha(U8 f){
		color.set_alpha(f);
	}
	void Model::set_color_and_flatness(U8 r, U8 g, U8 b, U8 f){
		color.set_color(r, g, b);
		color.set_alpha(f);

	}
	

	void Model::set_tiling(U32 u, U32 v){
		tiling = glm::vec2(u, v);
	}

	void Model::set_texture_normal(Texture t, Texture n){
		texture = t;
		normal_map = n;
	}

}