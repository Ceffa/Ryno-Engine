#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Global.h"
#include "TextureManager.h"
#include "Structures.h"
#include "Material.h"
#include <list>

#include "Script.h"

namespace Ryno{
	class SubModel;
	

	class Model : public Script{
	public:
	
		SubModel* add_sub_model();
		void remove_sub_model(SubModel* s);

		Model() {}
		~Model(){}
		void copy(const Model& cp);
		Model(const Model& cp);
		std::list<SubModel*> sub_models;

		void update() override {}
		void input() override {}
		void start() override {}
		bool is_unique() override { return true; };
		bool is_copyable() override { return true; };
		virtual Model* clone() {
			return new Model(*this);
		}

		
	};









	class SubModel {
		friend class Model;
	public:
		SubModel();
		~SubModel();
		SubModel(const SubModel& cp);
		void copy(const SubModel& cp);
		I32 mesh;
		Material material;
		bool cast_shadows;

		Model* parent_model;

		static std::list<SubModel*> submodels;
		static U32 nr_of_submodels;
	private:
		static void insert_ordered(SubModel* s);
		static void remove_ordered(SubModel* s);
		const static U8 compare_models(SubModel* a, SubModel* b);


	};
}