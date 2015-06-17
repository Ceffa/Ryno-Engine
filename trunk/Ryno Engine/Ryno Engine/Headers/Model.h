#pragma once
#include <vector>
#include <GLM\glm.hpp>
#include "Types.h"
#include "TextureLoader.h"
#include "Structures.h"

namespace Ryno{

	

	struct Mesh{
		std::vector<Vertex3D> vertices;
	};
	class Model{
	public:

		/**
		Load a model and set it's initial color
		@Param name -> name of the model in Resource folder
		@Paramm color -> initial color of the model. White is default.
		*/
		static void load_model(const std::string& name, Mesh* mesh, ColorRGBA color = ColorRGBA::ColorRGBA(1, 1, 1, 1));

		glm::mat4 model_matrix;
		GLTexture texture;
		Mesh mesh;


	};
}