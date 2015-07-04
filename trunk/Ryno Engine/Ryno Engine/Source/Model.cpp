#include "Model.h"


namespace Ryno{


	Model::Model(){
		color.set_color(255,255,255,255);
	}
	void Model::set_color(U8 r, U8 g, U8 b, U8 a){
		color.set_color(r, g, b, a);
	}

}