#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Global.h"
#include "TextureManager.h"
#include "Structures.h"
#include "Material.h"
#include <list>

namespace Ryno{
		
	class SubModel {
	public:
		SubModel() {}
		I32 mesh;
		Material material;
		bool cast_shadows;
		SubModel(const SubModel& cp);
		void copy(const SubModel& cp);
	};
	class Model{
	public:
	
		SubModel& add_sub_model();
		Model() { }
		~Model(){}
		void copy(const Model& cp);
		Model(const Model& cp);
		std::vector<SubModel> sub_models;
		
	};
}