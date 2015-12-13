#pragma once

#include "Global.h"
#include "Structures.h"
#include "GameObject.h"
#include "Camera3D.h"
#include "MeshManager.h"
#include "GLSLProgram.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{


	struct InputInstance{
		glm::mat4 m;

	};
	class RenderBatch{
	public:
		RenderBatch(U32 v_o, U32 n_v, U32 idx_o, U32 n_idx, U32 i_o, U32 n_i, U32 m) : vertex_offset(v_o), num_vertices(n_v), indices_offset(idx_o), num_indices(n_idx), instance_offset(i_o),num_instances(n_i), mesh(m){}
		U32 vertex_offset;
		U32 num_vertices;
		U32 indices_offset;
		U32 num_indices;
		U32 instance_offset;
		U32 num_instances;
		U32 mesh;
	};


	class Batch3DAbstract{
	public:
		
		void init(Camera3D* camera);
		void set_camera(Camera3D* camera);
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual void draw(GameObject* go) = 0;
		virtual void render_batch() = 0;

	protected:
		
		
		std::vector<GameObject*> m_game_objects;
		std::vector<U32> indices;

		virtual void create_render_batches() = 0;
		virtual void create_vertex_array() = 0;
		virtual void enable_attributes() = 0; 

		U32 m_vbo;
		U32 m_i_vbo;//instancing vbo
		U32 m_index_vbo;
		U32 m_vao;

		Camera3D* m_camera;
		MeshManager* m_mesh_manager;


	};





}