#pragma once

#include "Batch3DAbstract.h"
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


	struct InputInstanceGeometry : public InputInstance {
		glm::vec2 tiling;
		ColorRGBA color;
	};
	class RenderBatchGeometry : public RenderBatch{
	public:
		RenderBatchGeometry(U32 v_o, U32 m_o, U32 n_v, U32 n_i, U32 t,U32 n_m, U32 m) : RenderBatch(v_o, m_o,n_v,n_i,m), texture(t),normal_map(n_m){}
	
		U32 normal_map;
		U32 texture;
	};


	class Batch3DGeometry : public Batch3DAbstract{
	public:
		
		void begin() override;
		void end() override;
		
		void draw(GameObject* go) override;

		void render_batch() override;
		
	protected:

		std::vector<Vertex3D> vertices;
		std::vector<InputInstanceGeometry> input_instances;
		std::vector<RenderBatchGeometry> m_render_batches;
		
		static U8 compare_models(GameObject* a, GameObject* b) ;

		void create_render_batches();
		void create_vertex_array();
		void enable_attributes();
	};





}