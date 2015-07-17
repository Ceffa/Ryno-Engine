#include "Model.h"


namespace Ryno{


	Model::Model(){
		color.set_color_and_flatness(255,255,255,0);
		texture = -1;
		normal_map = -1;
		set_tiling(1, 1);
	}
	void Model::set_color(U8 r, U8 g, U8 b){
		color.set_color(r, g, b);
	}
	void Model::set_flatness(U8 f){
		color.set_flatness(f);
	}
	void Model::set_color_and_flatness(U8 r, U8 g, U8 b, U8 f){
		color.set_color(r, g, b);
		color.set_flatness(f);

	}
	

	void Model::set_tiling(U32 u, U32 v){
		tiling.set(u, v);
	}

	void Model::set_mesh_texture_normal(I32 m, I32 t, I32 n){
		mesh = m;
		texture = t;
		normal_map = n;
	}

}