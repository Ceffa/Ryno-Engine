#include "Model.h"


namespace Ryno{

	U32 SubModel::nr_of_submodels = 0;
	std::forward_list<SubModel*> SubModel::submodels;

	SubModel* Model::add_sub_model()
	{
		sub_models.push_back(new SubModel());
		SubModel* s = sub_models.back();
		s->parent_model = this;
		return s;
	}

	void Model::remove_sub_model(SubModel* s)
	{
		sub_models.remove(s);
	}

	void Model::copy(const Model& copy)
	{
		for (auto s : copy.sub_models) {
			sub_models.emplace_back(s);
		}
	}
	
	Model::Model(const Model& cp) {
		copy(cp);
	}

	

	SubModel::SubModel(const SubModel& cp):SubModel()
	{
		copy(cp);
	}

	SubModel::SubModel()
	{
		insert_ordered(this);
	}

	void SubModel::copy(const SubModel& cp) {
		cast_shadows = cp.cast_shadows;
		mesh = cp.mesh;
		material.copy(cp.material);
	}

	void SubModel::insert_ordered(SubModel* s)
	{
		nr_of_submodels++;
		for (auto& it = submodels.begin(); it != submodels.end(); it++) {
			if (compare_models(s, *it)) {
				submodels.insert_after(it, s);
				break;
			}
		}
	}

	void SubModel::remove_ordered(SubModel* s)
	{
		nr_of_submodels--;
		submodels.remove(s);
	}

	const U8 SubModel::compare_models(SubModel* a, SubModel* b) {
		const auto ma = a->material;
		const auto mb = b->material;

		if (ma.shader != mb.shader)
			return ma.shader < mb.shader;

		if (a->mesh != b->mesh)
			return a->mesh < b->mesh;

		auto ita = ma.uniform_map.begin();
		auto itb = mb.uniform_map.begin();

		while (ita != ma.uniform_map.end()) {
			I8 res = memcmp(ita->second, itb->second, ma.shader->uniforms_data[ita->first].size);
			if (res == 0) {
				ita++; itb++;
				continue;
			}
			return res < 0 ? true : false;
		}
		return false;
	}

	SubModel::~SubModel()
	{
		remove_ordered(this);
	}

}