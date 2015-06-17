#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Types.h"
#include "TextureLoader.h"
#include "Structures.h"

namespace Ryno{

	


	class Model{
	public:

		/**
		Load a model and set it's initial color
		@Param name -> name of the model in Resource folder
		@Paramm color -> initial color of the model. White is default.
		*/
		static Model* load_model(const std::string& name, ColorRGBA color = ColorRGBA::ColorRGBA(1,1,1,1));


		GLTexture texture;
		std::vector<Vertex3D> vertices;


	};
}