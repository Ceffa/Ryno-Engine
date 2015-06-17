#pragma once

#include "Types.h"
#include "Structures.h"
#include "Model.h"
#include "Camera3D.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{


	class RenderBatch{
	public:
		RenderBatch(U32 o, U32 n, U32 t) : offset(o), num_vertices(n), texture(t){}
		U32 offset;
		U32 num_vertices;
		U32 texture;
	};


	class Batch3D{
	public:
	
		void init();
		void begin();
		void end();
		void render_batch();
		
		void draw(const Model& model);
		
	private:

		//two vector to optimize access and sorting
		std::vector<Model*> m_models_pointers;
		std::vector<Model> m_models;
		std::vector<RenderBatch> m_render_batches;
		
		static U8 compare_texture(Model* a, Model* b);

		void create_render_batches();
		void create_vertex_array();

		U32 m_vbo;
		U32 m_vao;



	};





}