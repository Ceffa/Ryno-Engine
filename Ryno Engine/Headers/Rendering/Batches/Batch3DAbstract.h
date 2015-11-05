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
		RenderBatch(U32 v_o, U32 m_o, U32 n_v, U32 n_i, U32 m) : vertex_offset(v_o), instance_offset(m_o),num_vertices(n_v),num_instances(n_i), mesh(m){}
		U32 vertex_offset;
		U32 instance_offset;
		U32 num_vertices;
		U32 num_instances;
		U32 mesh;
	};


	class Batch3DAbstract{
	public:
		
		void init(Camera3D* camera);
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual void draw(GameObject* go) = 0;
		virtual void render_batch() = 0;

	protected:
		
		
		std::vector<GameObject*> m_game_objects;
		
		virtual void create_render_batches() = 0;
		virtual void create_vertex_array() = 0;
		virtual void enable_attributes() = 0; 

		U32 m_vbo;
		U32 m_i_vbo;//instancing vbo
		U32 m_vao;

		Camera3D* m_camera;
		MeshManager* m_mesh_manager;


	};





}