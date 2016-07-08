#pragma once

#include "Shader.h"
#include "Global.h"
#include "Structures.h"
#include "Model.h"
#include "Camera3D.h"
#include "MeshManager.h"
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{

	struct uniform{
		uniform(std::string n, void* v) : name(n),  value(v){}
		std::string name;
		void* value;
	};
	class RenderBatchStruct{
	public:
		RenderBatchStruct(U32 v_o, U32 n_v, U32 idx_o, U32 n_idx, U32 i_o, U32 n_i, SubModel* _m) : vertex_offset(v_o), num_vertices(n_v), indices_offset(idx_o), num_indices(n_idx), instance_offset(i_o), num_instances(n_i), model(_m){}
		U32 vertex_offset;
		U32 num_vertices;
		U32 indices_offset;
		U32 num_indices;
		U32 instance_offset;
		U32 num_instances;
		SubModel* model;
	};


	class Batch3DGeometry{
	public:
		Batch3DGeometry(){}
		void init();
		void begin();
		void end();
		
		void draw(SubModel* s);

		//Render batches with material-defined shaders
		void render_batch();

		std::list<Shader*> shaders;
		std::vector<SubModel*> m_models;

	protected:
		U32 m_vbo = 0;
		U32 m_i_vbo = 0;//instancing vbo
		U32 m_index_vbo = 0;
		U32 m_vao = 0;

		MeshManager* m_mesh_manager;

		std::vector<U32> indices;


		std::vector<Vertex3D> vertices;
		void* input_instances = nullptr;
		std::vector<RenderBatchStruct> m_render_batches;
		

		void create_render_batches();
		void enable_attributes(Shader* s);
		void create_vertex_array();
	};





}