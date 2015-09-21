#pragma once

#include "Batch3DAbstract.h"
#include "Global.h"
#include "Structures.h"
#include "Model.h"
#include "Camera3D.h"
#include "MeshManager.h"
#include "GLSLProgram.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace Ryno{

	//These two struct are identical to the superclass ones
	struct InputInstanceShadow : public InputInstance{
	};

	class RenderBatchShadow : public RenderBatch{
	public:
		RenderBatchShadow(U32 v_o, U32 m_o, U32 n_v, U32 n_i, U32 m) : RenderBatch(v_o, m_o, n_v, n_i, m){}
	
	};


	class Batch3DShadow : public Batch3DAbstract{
	public:
		
		void begin() override;
		void end() override;
		
		void draw(GameObject* go) override;

		void render_batch() override;


	protected:
		
		
		std::vector<glm::vec3> vertices_positions; //just position instead of whole vertex
		std::vector<InputInstanceShadow> input_instances;
		std::vector<RenderBatchShadow> m_render_batches;
		
		static U8 compare_models(GameObject* a, GameObject* b);

		void create_render_batches() override;
		void create_vertex_array() override;
		void enable_attributes() override;

	};





}