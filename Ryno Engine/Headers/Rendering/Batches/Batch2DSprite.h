#pragma once

#include <GLM/glm.hpp>
#include "Structures.h"
#include "Sprite.h"
#include <vector>

namespace Ryno{


	struct InstanceValues {
		glm::mat3 m;
		glm::vec2 tiling;
		ColorRGBA color;
		U8 depth;
	};
	class RenderBatch2D{
	public:
		RenderBatch2D(U32 num_ins, U32 text) :  num_instances(num_ins), texture(text){}
		U32 num_instances;
		U32 texture;
	};


	class Batch2DSprite{
	public:
		
		void init();
		void begin();
		void end();
		
		void draw(Sprite* sprite);

		void render_batch();
		
	protected:

		std::vector<Sprite*> m_sprites;
		U32 m_vbo=0;
		U32 m_i_vbo=0;//instancing vbo
		U32 m_indices_vbo=0;
		U32 m_vao=0;

		std::vector<InstanceValues> input_instances;
		std::vector<RenderBatch2D> m_render_batches;
		
		static U8 compare_models(Sprite* a, Sprite* b) ;

		void create_render_batches();
		void create_vertex_array();
		void enable_attributes();

		

		
	};





}