#pragma once

#include "Types.h"
#include "Structures.h"
#include "Model.h"
#include "Camera3D.h"
#include "MeshManager.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{

	struct InputInstance{
		
		glm::mat4 mvp;
		glm::mat4 normal_matrix;
		ColorRGBA color;

	};
	class RenderBatch{
	public:
		RenderBatch(U32 v_o, U32 m_o, U32 n_v, U32 n_i, U32 t, U32 m) : vertex_offset(v_o), instance_offset(m_o),num_vertices(n_v),num_instances(n_i), texture(t), mesh(m){}
		U32 vertex_offset;
		U32 instance_offset;
		U32 num_vertices;
		U32 num_instances;
		U32 texture;
		U32 mesh;
	};


	class Batch3D{
	public:
		
		void init(Camera3D* camera);
		void begin();
		void end();
		void render_batch();
		
		void draw(Model* model);
		
	private:

		//two vector to optimize access and sorting
		std::vector<Model*> m_models;
		std::vector<InputInstance> input_instances;
		std::vector<RenderBatch> m_render_batches;
		
		static U8 compare_texture(Model* a, Model* b);

		void create_render_batches();
		void create_vertex_array();

		U32 m_vbo;
		U32 m_i_vbo;//instancing vbo
		U32 m_vao;

		Camera3D* m_camera;
		MeshManager& m_mesh_loader = MeshManager::get_instance();


	};





}