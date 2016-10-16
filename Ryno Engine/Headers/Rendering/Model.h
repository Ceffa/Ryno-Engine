#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Global.h"
#include "TextureManager.h"
#include "Structures.h"
#include "Material.h"
#include <set>
#include "Script.h"

namespace Ryno{
	class Model;
	class SubModel {
	public:
		SubModel() {}
		I32 mesh;
		Material material;
		bool cast_shadows;
		SubModel(const SubModel& cp);
		void copy(const SubModel& cp);
		Model* parent;
	};
	class Model : public Script{
	public:
	
		SubModel& add_sub_model();

		Model() { models.insert(this); }
		~Model() { models.erase(this); }
		void copy(const Model& cp);
		Model(const Model& cp);
		std::vector<SubModel> sub_models;
		static std::set<Model*> models;

		void update() override {}
		void input() override {}
		void start() override {}
		bool is_unique() override { return true; };
		bool is_copyable() override { return true; };
		virtual Model* clone() {
			return new Model(*this);
		}

	};
}