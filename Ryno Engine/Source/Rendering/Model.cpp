#include "Model.h"


namespace Ryno{

	


	SubModel& Model::add_sub_model()
	{
		sub_models.emplace_back();

		return sub_models.back();
	}

	void Model::copy(const Model& copy)
	{
		for (const SubModel& s : copy.sub_models) {
			sub_models.emplace_back(s);
		}
	}
	
	Model::Model(const Model& cp) {
		copy(cp);
	}

	

	SubModel::SubModel(const SubModel& cp)
	{
		copy(cp);
	}

	void SubModel::copy(const SubModel& cp) {
		cast_shadows = cp.cast_shadows;
		mesh = cp.mesh;
		material.copy(cp.material);
	}

}