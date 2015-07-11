#include "Model.h"


namespace Ryno{


	Model::Model(){
		color.set_color(255,255,255,255);
		texture = -1;
		normal_map = -1;
		set_tiling(1, 1);
	}
	void Model::set_color(U8 r, U8 g, U8 b, U8 a){
		color.set_color(r, g, b, a);
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