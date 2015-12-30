#include "Model.h"


namespace Ryno{


	Model::Model(const Model& copy) : material(Material(copy.material))
	{
		cast_shadows = copy.cast_shadows;
		mesh = copy.mesh;
	}

	

}