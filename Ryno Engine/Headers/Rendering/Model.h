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
		SubModel(const SubModel& copy);
	};
	class Model{
	public:
	
		SubModel& add_sub_model();
		Model() { }
		~Model(){}
		Model(const Model& copy);
		std::vector<SubModel> sub_models;
		
	};
}