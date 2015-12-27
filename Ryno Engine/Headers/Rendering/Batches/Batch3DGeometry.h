#pragma once

#include "Batch3DAbstract.h"
#include "Shader.h"
#include "Global.h"
#include "Structures.h"
#include "GameObject.h"
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
	class RenderBatchGeometry : public RenderBatch{
	public:
		RenderBatchGeometry(U32 v_o, U32 n_v, U32 idx_o, U32 n_idx, U32 i_o, U32 n_i, Model* mod) : RenderBatch(v_o, n_v, idx_o, n_idx, i_o, n_i), model(mod){}
		Model* model;
	};


	class Batch3DGeometry : public Batch3DAbstract{
	public:
		
		void begin() override;
		void end() override;
		
		void draw(GameObject* go) override;

		void render_batch() override;
		std::list<Shader*> shaders;

	protected:

		std::vector<Vertex3D> vertices;
		void* input_instances;
		std::vector<RenderBatchGeometry> m_render_batches;
		
		const static U8 compare_models(GameObject* a, GameObject* b) ;

		void create_render_batches();
		void enable_attributes(Shader* s);
		void create_vertex_array();
	};





}