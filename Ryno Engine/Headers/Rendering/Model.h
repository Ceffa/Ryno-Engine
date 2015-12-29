#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Global.h"
#include "TextureManager.h"
#include "Structures.h"
#include "Material.h"
#include <list>

namespace Ryno{
		
	class Model{
	public:
	

		Model(){}
		~Model(){}
		Model(const Model& copy);
		I32 mesh;
		bool cast_shadows = true;
		New<Material> material;
		
	};
}