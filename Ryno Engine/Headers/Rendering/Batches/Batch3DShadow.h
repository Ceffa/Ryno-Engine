#pragma once

#include "Global.h"
#include "Structures.h"
#include "Model.h"
#include "Camera3D.h"
#include "MeshManager.h"
#include "GameObject.h"
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{


	class RenderBatchShadow{
	public:
		RenderBatchShadow(U32 v_o, U32 n_v, U32 idx_o, U32 n_idx, U32 i_o, U32 n_i, SubModel* mod) : vertex_offset(v_o), num_vertices(n_v), indices_offset(idx_o), num_indices(n_idx), instance_offset(i_o), num_instances(n_i), model(mod){}
		U32 vertex_offset;
		U32 num_vertices;
		U32 indices_offset;
		U32 num_indices;
		U32 instance_offset;
		U32 num_instances;
		SubModel* model;
	};

	struct model_and_matrix {
	public:
		model_and_matrix(SubModel* s, glm::mat4* m) : model(s), matrix(m) {}
		SubModel* model;
		glm::mat4* matrix;
	};

	class Batch3DShadow{
	public:
		
		void begin();
		void end();
		
		void init(Camera3D* camera);
		void set_camera(Camera3D* camera);

		void draw(Model* m);

		void render_batch();

		std::vector<model_and_matrix> m_models;

	private:
		
		
		std::vector<glm::vec3> vertices_positions; //just position instead of whole vertex
		std::vector<glm::mat4> models_matrices;
		std::vector<RenderBatchShadow> m_render_batches;
		
		static U8 compare_models(model_and_matrix a, model_and_matrix b);
		
		std::vector<U32> indices;

		void create_render_batches();
		void create_vertex_array();
		void enable_attributes();
		U32 m_vbo=0;
		U32 m_i_vbo=0;//instancing vbo
		U32 m_index_vbo=0;
		U32 m_vao=0;

		Camera3D* m_camera;
		MeshManager* m_mesh_manager;


	};





}