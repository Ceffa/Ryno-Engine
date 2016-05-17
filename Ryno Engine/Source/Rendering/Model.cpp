#include "Model.h"


namespace Ryno{



	SubModel& Model::add_sub_model()
	{
		sub_models.emplace_back();
		return sub_models.back();
	}

	Model::Model(const Model& copy)
	{
		for (const SubModel& s : copy.sub_models) {
			sub_models.emplace_back(s);
		}

	
	}

	

	SubModel::SubModel(const SubModel& cp)
	{
		cast_shadows = cp.cast_shadows;
		mesh = cp.mesh;
		material.copy(cp.material);
	}

}