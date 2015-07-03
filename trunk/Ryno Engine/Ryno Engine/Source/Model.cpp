#include "Model.h"


namespace Ryno{


	Model::Model(){
		color.set_color(1, 1, 1, 1);
	}
	void Model::set_color(U8 r, U8 g, U8 b, U8 a){
		color.set_color(r, g, b, a);
	}

}